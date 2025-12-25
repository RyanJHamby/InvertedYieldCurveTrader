# Macro Surprise-Driven Risk Decomposition Framework

## The Big Idea

**Markets don't react to economic levels. They react to surprises, conditioned on risk appetite.**

A CPI print of 3.1% is irrelevant alone. The market cares:
- *Was that expected 3.5% (bullish shock) or 2.8% (bearish shock)?* ← **Surprise**
- *Was this printed in a high-VIX, inverted-curve regime or low-vol, steep-curve?* ← **Market state**

This framework:
1. Extracts **discrete macro shocks** (expectations-adjusted innovations)
2. Captures **continuous market state** (volatility, spreads, rates—not sentiment vibes)
3. Derives **regime variables** (risk-on/off, inflation-sensitive, fragile/stable)
4. Decomposes shocks into **orthogonal factors** (growth, inflation, policy, volatility)
5. Attributes portfolio risk to **macro factors conditioned on state**

It answers:

> *"Which macro shock hit us, in what regime, and why did our portfolio break?"*

---

## Why This Matters for Risk Management

**The problem with level-based systems:**
- Confuse trends with shocks
- Miss regime transitions
- Can't explain drawdowns post-mortem
- False correlations from slow-moving state variables

**What this framework does:**
- Isolates pure information shocks
- Decomposes portfolio risk into interpretable factors (growth, inflation, policy)
- Explains 2008, 2020, 2022 drawdowns by economic cause
- Enables risk-aware sizing *before* the shock hits

**Who cares:** Risk managers, macro PMs, hedge funds managing tail risk, discretionary traders who need to understand what broke their portfolio.

---

## Data Architecture: Three Layers (The Secret Sauce)

This is where amateurs fail. You need **three distinct objects**, not one monolithic "indicator list."

### Layer A: Discrete Information Shocks (Sparse, Event-Driven)

**What it is:** Expectation-adjusted macro innovations at release time.

```cpp
struct MacroShock {
    std::string indicator;    // "CPI", "NFP", "GDP", "FOMC"
    double innovation;        // X_t - E[X_t] (zero-mean by construction)
    double timestamp;         // Release time
    std::string source;       // "survey" or "model"
};
```

**Properties:**
- Zero-mean (if expectations are rational)
- Sparse in time (only at releases)
- Orthogonalized together (they're the "signal")

**Do NOT:**
- Use these to predict continuously
- Treat them like state variables

---

### Layer B: Continuous Market State (Daily/Hourly)

**What it is:** How fragile is the market when shocks arrive?

**Include (all free, FRED/CBOE):**

| Variable | Why | Free Source |
|----------|-----|---|
| VIX level | Real-time fear gauge | FRED |
| VIX term structure (1M/3M ratio) | Near-term vs forward positioning | CBOE |
| MOVE index | Bond market fear (often leads equities) | FRED |
| BAA–AAA spread | Credit risk appetite | FRED |
| 2s10s yield curve slope | Growth expectations | FRED |
| Put/Call ratio (smoothed) | Option market positioning | Yahoo/CBOE |

```cpp
struct MarketState {
    double vixLevel;
    double vixSlope;          // VIX1M / VIX3M
    double move;
    double creditSpread;
    double yieldCurveSlope;
    double putCallRatioSmoothed;
    Timestamp t;
};
```

**Do NOT:**
- Orthogonalize these against shocks (that's backwards)
- Treat them as alpha signals
- Blend them with macro shocks

**DO:**
- Use them to condition shock impact
- Example: CPI innovation × high-MOVE regime ≠ CPI innovation × low-MOVE regime

---

### Layer C: Derived Regime Variables (Compressed State)

**What it is:** Clustered, interpretable market conditions.

From Layer B, derive:

* Risk-On / Risk-Off regime
* Inflation-sensitive / Growth-sensitive regime
* Stable / Fragile regime

```cpp
struct RiskRegime {
    std::string label;        // "Risk-Off + Fragile + Inflation-Sensitive"
    double confidence;        // 0.0–1.0
    Timestamp t;
};
```

Use clustering (k-means) or rules-based thresholds on Layer B variables.

---

## Key Architectural Principle

**Shocks** (Layer A) are **orthogonalized**.
**State** (Layer B) is **not**.
**Regimes** (Layer C) **condition impact** of Layer A.

That's it. That's the whole system.

---

## Phase 0: Foundation (COMPLETE ✅)

### Why We Start Here

Before extracting surprises, we need:
- Clean macro data at aligned frequencies
- A volatility baseline to measure shocks against
- Proof that our alignment and covariance math is correct

### Step 1.1: Macro Data Backbone + Regime Volatility Proxy ✅

**What was built:**
- FRED API (5 core economic indicators)
- VIX / Alpha Vantage integration
- Monthly frequency alignment (daily → monthly downsampling, quarterly → monthly interpolation)
- 8×8 covariance matrix
- Frobenius norm as regime volatility signal

**Why it matters:**
- Establishes a clean foundation for shock extraction
- 73 unit tests verify alignment and covariance correctness
- No surprises yet, just baselines

**Code:** `src/DataProcessors/{DataAligner, CovarianceCalculator}.cpp`

---

## Phase 1: Shock Extraction & Risk Attribution (NEXT)

### Why This Phase Changes Everything

Right now, we covariance indicator **levels**. That's weak.

A 3.5% unemployment rate has no information. A surprise *drop* to 3.3% when markets expected 3.4% is a growth shock worth billions in ES vol.

This phase converts levels → shocks, creating the signal that actually moves prices.

---

### Step 1.2: Macro Surprise Construction ⏳ **NEXT PRIORITY**

**Goal:** Extract information shocks from economic releases.

**The Math (simple):**
```
ε_t = X_t − E[X_t]
```

Where:
- X_t = actual release (e.g., CPI 3.1%)
- E[X_t] = market expectation (e.g., 3.5% consensus)
- ε_t = surprise (−0.4% = bearish shock)

**Key insight:** Surprises have near-zero mean over time (if expectations are good), and their *covariance* captures correlated macro shocks, not correlated trends.

---

#### Implementation: `SurpriseTransformer`

**File:** `src/DataProcessors/SurpriseTransformer.hpp`

```cpp
struct IndicatorSurprise {
    std::vector<double> surprises;      // ε_t (should be mean ≈ 0)
    std::vector<double> rawValues;      // X_t
    std::vector<double> expectations;   // E[X_t]
    std::string expectationSource;      // "survey", "ar1_forecast", "previous"
    bool isValidated;                   // mean(ε) close to 0?
};

class SurpriseTransformer {
public:
    // Core method: convert levels → surprises
    static IndicatorSurprise extractSurprise(
        const std::vector<double>& levels,
        const std::string& indicator,
        int lookbackMonths = 12
    );

    // Validate that surprises have zero mean
    // (If not, expectations were systematically biased)
    static bool validateZeroMean(
        const IndicatorSurprise& surprise,
        double tolerance = 0.05
    );

private:
    // Expectation sources (tried in order):
    // 1. Survey consensus (Bloomberg, Wall Street Journal median)
    // 2. AR(1) rolling forecast (if no survey)
    // 3. Previous release (for low-frequency data like CPI)
    // 4. Zero (first data point)

    static std::vector<double> loadSurveyExpectations(const std::string& indicator);
    static double forecastAR1(const std::vector<double>& history);
};
```

**File:** `src/DataProcessors/SurpriseTransformer.cpp`

**Core logic:**

```cpp
IndicatorSurprise SurpriseTransformer::extractSurprise(
    const std::vector<double>& levels,
    const std::string& indicator,
    int lookbackMonths)
{
    IndicatorSurprise result;
    result.rawValues = levels;
    result.surprises.resize(levels.size());
    result.expectations.resize(levels.size());

    for (size_t i = 0; i < levels.size(); i++) {
        double expected = 0.0;
        std::string source = "none";

        // 1. Try survey data first (most accurate market expectations)
        auto surveyData = loadSurveyExpectations(indicator);
        if (i < surveyData.size() && surveyData[i] != 0) {
            expected = surveyData[i];
            source = "survey";
        }
        // 2. Fall back to AR(1) forecast (self-generating expectations)
        else if (i >= lookbackMonths) {
            std::vector<double> history(levels.begin(), levels.begin() + i);
            expected = forecastAR1(history);
            source = "ar1_forecast";
        }
        // 3. Or use previous release (crude but works for sticky data)
        else if (i > 0) {
            expected = levels[i - 1];
            source = "previous";
        }
        // 4. Default to zero
        else {
            expected = 0.0;
            source = "none";
        }

        result.expectations[i] = expected;
        result.surprises[i] = levels[i] - expected;
    }

    // Check zero-mean property
    double mean = 0.0;
    for (double s : result.surprises) mean += s;
    mean /= result.surprises.size();
    result.isValidated = (std::abs(mean) < 0.05);
    result.expectationSource = source;

    return result;
}

double SurpriseTransformer::forecastAR1(const std::vector<double>& history) {
    // AR(1): X_t = α + β X_{t-1} + noise
    // Estimated via rolling regression on past 12 months
    if (history.size() < 2) return 0.0;

    double sumXY = 0.0, sumX2 = 0.0, meanX = 0.0, meanY = 0.0;
    int n = history.size() - 1;

    for (int i = 0; i < n; i++) {
        meanX += history[i];
        meanY += history[i + 1];
    }
    meanX /= n;
    meanY /= n;

    for (int i = 0; i < n; i++) {
        double x = history[i] - meanX;
        double y = history[i + 1] - meanY;
        sumXY += x * y;
        sumX2 += x * x;
    }

    double beta = (sumX2 > 1e-10) ? sumXY / sumX2 : 0.0;
    double alpha = meanY - beta * meanX;
    return alpha + beta * history.back();
}
```

**Why this matters:**

This single step fixes the biggest weakness in the current system:
- Separates trends (slow-moving levels) from shocks (surprises)
- Enables covariance to capture *correlated macro shocks*, not correlated drifts
- Proves whether expectations were rational (zero-mean surprises)
- Serves as input to all downstream analysis

**Unit tests (target: 15+ tests):**

```cpp
// Surprises should be zero-mean (if expectations were rational)
TEST(SurpriseTransformerTest, ZeroMeanProperty) {
    std::vector<double> levels = {100, 102, 101, 103, 99, 104, 102, 105};
    auto surprise = SurpriseTransformer::extractSurprise(levels, "test");

    double mean = 0.0;
    for (auto s : surprise.surprises) mean += s;
    mean /= surprise.surprises.size();

    EXPECT_NEAR(mean, 0.0, 0.05);
    EXPECT_TRUE(surprise.isValidated);
}

// AR(1) forecast should be reasonable
TEST(SurpriseTransformerTest, AR1ForecastIsRational) {
    std::vector<double> levels = {100, 101, 102, 103, 104, 105};
    auto surprise = SurpriseTransformer::extractSurprise(levels, "test");

    // Uptrend → AR(1) should forecast ~106
    EXPECT_GT(surprise.expectations.back(), 105.0);
}

// Validation should fail for strongly biased expectations
TEST(SurpriseTransformerTest, ValidationFailsOnBias) {
    std::vector<double> levels = {1000, 1001, 1002, 1003, 1004, 1005};
    auto surprise = SurpriseTransformer::extractSurprise(levels, "test");
    EXPECT_FALSE(surprise.isValidated);  // Strongly trending, not zero-mean
}

// Survey data should be preferred over AR(1)
TEST(SurpriseTransformerTest, SurveyTakesPrecedence) {
    // Mock: CPI actual 3.1%, survey expected 3.5%, AR(1) predicted 3.0%
    // Surprise should be −0.4% (vs actual − survey), not +0.1%
}
```

---

## Phase 2: Macro Factor Discovery (INTERPRETABILITY)

### Why Factor Decomposition Matters

The 8×8 covariance matrix of surprises has 36 unique entries. That's noise and overfitting.

Economic theory says 3–4 factors drive everything:
- **Growth:** Impacts GDP, employment, sentiment; inverse on unemployment
- **Inflation:** CPI, wage pressure; inverse on real rates
- **Policy:** Fed funds, central bank actions; directly impacts spreads
- **Volatility:** VIX shocks; tail risk

This phase **compresses 8 dimensions → 3–4 factors** without losing signal.

---

### Step 2.1: Surprise-Based Factor Decomposition ⏳ **AFTER 1.2**

**Goal:** PCA on orthogonalized macro shocks → interpretable, stable factors.

**The Math (Exact, Not Hand-Wavy):**

Given orthogonalized shock vector ε_t ∈ ℝ^N:

```
ε_t = B f_t + u_t
```

Where:
- B ∈ ℝ^(N×K) = factor loadings (K ≈ 3–4 factors)
- f_t ∈ ℝ^K = macro factor shocks (orthogonal by construction)
- u_t = idiosyncratic shock noise

Covariance decomposition:
```
Σ_ε = B Σ_f B^T + Σ_u
```

**Deliverable:**

```cpp
struct MacroFactors {
    std::vector<Eigen::VectorXd> factors;           // Factor time series (K × T)
    Eigen::MatrixXd loadings;                       // B matrix (8 × K)
    std::vector<double> factorVariances;            // Var(f_k)
    std::vector<std::string> factorLabels;          // "Growth", "Inflation", etc.
    std::vector<double> labelConfidence;            // 0.0–1.0 per label
    double cumulativeVarianceExplained;             // Should be >70%
    Eigen::MatrixXd residualCovariance;             // Σ_u (leftover macro variance)
};
```

**Implementation (Rigorous):**

```cpp
class MacroFactorModel {
public:
    // PCA decomposition with stability tracking
    static MacroFactors decomposeSurpriseCovariance(
        const CovarianceMatrix& surpriseCov,
        int numFactors = 3
    );

    // Label factors via cosine similarity to economic archetypes
    // (Not post-hoc heuristics)
    static struct LabelResult {
        std::string label;
        double cosineScore;
        bool isStable;  // Did label change from last window?
    } labelFactorRobustly(
        const Eigen::VectorXd& loading,
        const std::vector<std::string>& indicatorNames,
        const LabelResult& previousLabel = {}
    );

    // Rolling decomposition with change detection
    static std::vector<MacroFactors> rollingDecompositionWithDriftDetection(
        const std::map<std::string, std::vector<double>>& surprises,
        int windowMonths = 12,
        int numFactors = 3,
        double stabilityThreshold = 0.85
    );
};
```

**Labeling Strategy (This is where juniors fail):**

Define economic **archetypes** (fixed):

```cpp
std::map<std::string, Eigen::VectorXd> economicArchetypes = {
    {"Growth",     {gdp: +1.0, unemployment: -1.0, sentiment: +1.0, inflation: 0.0, rates: 0.0}},
    {"Inflation",  {inflation: +1.0, wages: +1.0, gdp: 0.2, rates: 0.2}},
    {"Policy",     {rates: +1.0, spreads: +1.0, inflation: 0.1}},
    {"Volatility", {vix: +1.0, move: +1.0}}
};
```

For each PCA factor (b_k), compute:
```
score[j] = cosine_similarity(b_k, archetype[j])
```

Label = archetype with **maximum score**.

**Guardrails (CRITICAL):**

1. **Stability check:** If cosine(b_k^(t), b_k^(t-1)) < 0.85 → label = "Unstable"
2. **Confidence threshold:** Only assign label if max_score > 0.65, else = "Unclassified"
3. **Drift alert:** If label flips, freeze factor sizing until human review

This prevents **silent factor rotation** that breaks factor attribution.

**Why this matters:**

Traders don't hedge "CPI × unemployment × sentiment."
They hedge **growth risk**.

This phase translates **mathematically rigorous** factors into **stable business language**, with explicit confidence bounds.

---

## Phase 3: Portfolio Risk Attribution (THE MONEY QUESTION)

### Why Risk Attribution Wins

After a 5% drawdown, a PM asks: *"Was that a growth shock? Inflation shock? Volatility? Policy?"*

A system that answers this has real value. It explains history and informs future positioning.

---

### Step 3.1: Factor-Based Risk Decomposition ⏳ **AFTER 2.1**

**Goal:** Explain portfolio volatility and drawdowns via macro factor exposure, conditioned on market state.

**The Exact Math (No Hand-Waving):**

**Asset exposure to macro factor shocks:**

Given factor representation:
```
ε_t = B f_t + u_t
```

Portfolio return decomposition:
```
r_t = β^T ε_t + η_t = (β^T B) f_t + residual
```

Define:
```
γ = B^T β  (factor sensitivities)
```

So:
```
r_t = γ^T f_t + noise
```

**Portfolio variance:**
```
Var(r) = γ^T Σ_f γ + residual_variance
```

**Risk contribution of factor k:**
```
RC_k = γ_k (Σ_f γ)_k
```

Verify:
```
Σ_k RC_k = γ^T Σ_f γ  (sum to total)
```

**Conditioning on Market State (The Real Alpha):**

Factor volatility changes with regime:
```
Var(r | State_t) = γ^T Σ_f(State_t) γ
```

Example:
- CPI shock in high-MOVE regime: 3× risk impact
- Same shock in low-vol regime: baseline impact

**Implementation:**

```cpp
class PortfolioRiskAnalyzer {
public:
    struct RiskDecomposition {
        std::vector<double> factorSensitivities;     // γ_k
        std::vector<double> factorRiskContributions; // RC_k
        std::vector<std::string> factorLabels;
        double totalRisk;
        RiskRegime regime;                            // Conditioning state
    };

    // Exact factor attribution
    static RiskDecomposition analyzeRisk(
        const Eigen::VectorXd& assetSensitivities,   // β
        const MacroFactors& factors,                 // B, Σ_f
        const RiskRegime& state                      // Regime-dependent Σ_f
    );

    // Scenario analysis: shock impact given current regime
    static double scenarioShockImpact(
        const Eigen::VectorXd& assetSensitivities,
        const Eigen::VectorXd& shockVector,          // hypothetical f_t
        const MacroFactors& factors,
        const RiskRegime& currentRegime
    );

    // Crisis decomposition: which factors caused losses?
    static RiskDecomposition explainHistoricalDrawdown(
        const std::vector<double> returnsTo2008,    // Historical ES returns
        const std::vector<double> returnsTo2020,
        const std::vector<double> returnsTo2022,
        const std::vector<MacroFactors>& factors,
        const std::vector<RiskRegime>& regimesOverTime
    );

    // Rolling attribution with state awareness
    static std::vector<RiskDecomposition> rollingRiskDecomposition(
        const Eigen::VectorXd& assetSensitivities,
        const std::vector<MacroFactors>& factorsOverTime,
        const std::vector<RiskRegime>& regimesOverTime,
        int windowMonths = 12
    );
};
```

**What this enables:**

1. **Post-mortem clarity (the money question):**
   - 2008: 70% loss from growth collapse, 20% from policy tightening, 10% from tail risk
   - 2020: 80% from volatility shock, 15% from growth downside, 5% policy
   - 2022: 60% inflation regime loss, 25% policy shock, 15% growth

2. **Ex-ante risk management:**
   - "We're long ES, 65% exposed to growth shocks, 25% inflation, 10% policy"
   - "If growth shocks 2σ in high-MOVE regime, we lose 8% of portfolio"
   - Size accordingly

3. **Regime-specific hedging:**
   - Inflation regime: Buy inflation puts, reduce size
   - Easing regime: Growth shocks matter most, hedge accordingly
   - Tightening: Policy shocks dominate, monitor spreads

**Why this matters:**

This is where you become a **risk thinker**, not a signal chaser. Trading firms hire for this.

---

## Phase 4: Factor-Aware Positioning (OPTIONAL, STRATEGIC)

### Step 4.1: Risk-Constrained ES Exposure ⏳ **AFTER 3.1**

**Goal:** Position sizing that respects macro regime and factor exposure.

**NOT a signal engine.** This is risk management.

**Principle:**
- Size ES exposure conditional on factor volatility
- Reduce size during high-uncertainty regimes (multiple factors elevated)
- Explicitly hedge identified factor risks (e.g., buy inflation protection during tightening)
- Answer: *"How much ES should we own given current macro risk?"*

**Example sizing rule:**

```
ES Position = Base Size × (1 / Factor Volatility Multiplier)

Where Factor Volatility Multiplier =
    (1 + w_growth × Vol_growth + w_inflation × Vol_inflation + ...)
```

If growth shocks are 2σ high, reduce size. Simple.

---

## Known Blowup Scenarios (and Mitigations)

You will encounter these in live trading. Plan now.

### Risk 1: Endogeneity Loop

**What happens:**
- VIX reacts to CPI shock
- Your model "explains" VIX with CPI
- You double-count risk
- Shock impact explodes during crises

**Symptom:** Everything looks "inflation-driven" when it's actually just volatility spike.

**Mitigation:**
- Use **pre-event market state** (VIX at t−1, before CPI release)
- Never condition on post-shock volatility
- Rule: State ← t−ε, Shock → t, Impact measure at t+ε

---

### Risk 2: Factor Label Drift

**What happens:**
- PCA factors rotate silently in rolling windows
- "Inflation factor" becomes "Everything broke"
- Historical interpretation becomes invalid

**Symptom:** Backtests explain 2008–2022 perfectly, then label flips and forward testing collapses.

**Mitigation (CRITICAL):**
- Track cosine stability (archetypes vs. actual loadings)
- If cosine < 0.85 → label = "Unstable"
- Freeze sizing when labels flip
- Monthly review of factor stability

---

### Risk 3: Regime Classification Whippage

**What happens:**
- Regime classifier flips daily
- Risk sizing oscillates
- Over-trading, transaction costs

**Symptom:** Size jumps 50% every other day.

**Mitigation:**
- Add hysteresis (regime "stickiness"): don't flip until 2+ consecutive days in new regime
- Confidence thresholds: only act on regime shifts > 0.7 confidence
- Minimum regime duration: stay in regime for 5+ days minimum

---

### Risk 4: "Perfect Explanation, Zero Predictive Power"

**What happens:**
- Model perfectly explains 2008, 2020, 2022 drawdowns
- Implies you're just fitting history
- Forward returns uncorrelated with factor exposures

**Reality:** Risk attribution ≠ risk prediction.

**Mitigation:**
- Be explicit: this is **constraint system**, not prediction system
- Use for sizing caps, not return targets
- Hedge identified factor risks, don't take leverage
- Test on blind hold-out periods (2023–2025) before claiming robustness

---

### Risk 5: Sparse Shock Data

**What happens:**
- Only ~240 macro shocks per year
- Covariance matrices are unstable
- Factor loadings bounce around

**Mitigation:**
- Use 12–24 month windows minimum (not monthly)
- Regularize covariance (shrinkage estimators)
- Accept residual covariance as real (don't force all variance into factors)
- Don't claim precision you don't have

---

## What This Signals to Hiring Teams

**Implicit:** Without saying it explicitly, this system demonstrates:

✅ You understand macro shocks vs trends (levels vs surprises)

✅ You think in factors and regimes, not signals

✅ You care about drawdowns and tail risk, not Sharpe cosplay

✅ You can explain risk in language traders use ("growth shock", "inflation regime")

✅ You build systems that survive regime shifts

✅ You prioritize interpretability over black boxes

**That** is what elite macro, quant, and risk teams look for.

---

## Timeline & Complexity

| Phase | Step | Focus | Complexity | Impact | Est. Time |
|-------|------|-------|-----------|--------|-----------|
| 0 | 1.1 | Data backbone + covariance | ✅ Complete | Foundation | Done |
| 1 | **1.2** | **Surprise extraction** | Medium | **Huge** (fixes core problem) | 2–3 days |
| 2 | **2.1** | **PCA factor decomposition** | Medium | **High** (interpretability) | 3–4 days |
| 3 | **3.1** | **Risk attribution** | Medium | **Very High** (explainability) | 4–5 days |
| 4 | **4.1** | **Risk-aware sizing** | Medium | Depends on 3.1 | 2–3 days |

**Recommendation:** 1.2 → 2.1 → 3.1 in strict sequence. Each unlocks the next.

---

## What Winning Looks Like

After step 3.1, you can say in an interview:

> *"We separate macro shocks from market state. On March 2020, we show via attribution that growth expectations repriced (−2σ GDP shock) while volatility spiked. Our model was 65% growth-exposed, 25% volatility-exposed, 10% policy-exposed. That attribution proved correct: we lost 3.2% that day, decomposed as 2.1% growth shock, 0.8% volatility shock, 0.3% policy. We sized accordingly the next day based on regime."*

After step 4.1:

> *"We don't predict returns. We measure macro factor exposure and condition risk on market state (VIX, MOVE, spreads, curve). During 2022's inflation shock, we monitored in real-time that we were 70% exposed to inflation-policy shocks in a high-MOVE regime. That's 3× baseline risk. We reduced ES position by 30% to stay within target risk bands."*

**Resume bullet (one-liner):**

> Built macro risk decomposition system that orthogonalizes economic shocks into interpretable factors, conditions impact on market-implied risk regimes, and attributes portfolio drawdowns to economic causes—achieving explainable, regime-aware risk management without prediction claims.

That's a system trading firms hire for.

---

## Code Structure

```
src/DataProcessors/
  ├── SurpriseTransformer.cpp/hpp         (Step 1.2)
  ├── MacroFactorModel.cpp/hpp            (Step 2.1)
  └── PortfolioRiskAnalyzer.cpp/hpp       (Step 3.1)

test/
  ├── SurpriseTransformerUnitTest.cpp     (15+ tests)
  ├── MacroFactorModelUnitTest.cpp        (12+ tests)
  └── PortfolioRiskAnalyzerUnitTest.cpp   (10+ tests)
```

---

## Success Criteria

- [ ] **Step 1.2:** Surprises validate zero-mean; source attribution works
- [ ] **Step 2.1:** 3–4 factors explain >70% variance; labels are economically sensible
- [ ] **Step 3.1:** Can explain 2008, 2020, 2022 drawdowns by factor
- [ ] **Step 4.1:** Risk-aware sizing reduces drawdowns vs. static sizing
- [ ] **All:** 40+ new unit tests; no API keys required for testing

---

## Next Move

Start with **Step 1.2: Macro Surprise Extraction.**

This single step upgrades your project more than any other piece of work.

It changes the framing from *"macro analytics library"* to *"macro shock detection system."*
