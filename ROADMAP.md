# Macro Surprise-Driven Risk Decomposition Framework

## The Big Idea

**Markets don't react to economic levels. They react to surprises.**

A CPI print of 3.1% is irrelevant. The market cares: *was that expected 3.5% (bullish shock) or 2.8% (bearish shock)?*

This framework replaces covariance on **levels** with covariance on **shocks**, compresses noisy indicators into orthogonal **risk factors**, and answers the question traders ask after losses:

> *"Which macro shock hit us, and how hard?"*

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

**Goal:** PCA on surprise covariance → interpretable macro risk factors.

**Deliverable:**

```cpp
struct MacroFactors {
    std::vector<Eigen::VectorXd> factors;           // 3–4 factor time series
    Eigen::MatrixXd loadings;                       // Which indicators load on which factors
    std::vector<double> factorVariances;            // % of total variance per factor
    std::vector<std::string> factorLabels;          // "Growth", "Inflation", "Policy", "Volatility"
    double cumulativeVarianceExplained;             // Should be >70%
};
```

**Implementation outline:**

```cpp
class MacroFactorModel {
public:
    // Decompose surprise covariance using PCA
    static MacroFactors decomposeSurpriseCovariance(
        const CovarianceMatrix& surpriseCov,
        int numFactors = 3
    );

    // Economically label factors based on loadings
    // (Which indicators have highest weights?)
    static std::string labelFactorEconomically(
        const Eigen::VectorXd& loading,
        const std::vector<std::string>& indicatorNames
    );

    // Rolling-window decomposition for regime tracking
    static std::vector<MacroFactors> rollingFactorDecomposition(
        const std::map<std::string, std::vector<double>>& surprises,
        int windowMonths = 12,
        int numFactors = 3
    );
};
```

**Why this matters:**

Traders and risk managers don't hedge "GDP × unemployment × sentiment."

They hedge **growth risk**: "We're long equities, short growth."

This phase translates noisy correlations into business language.

---

## Phase 3: Portfolio Risk Attribution (THE MONEY QUESTION)

### Why Risk Attribution Wins

After a 5% drawdown, a PM asks: *"Was that a growth shock? Inflation shock? Volatility? Policy?"*

A system that answers this has real value. It explains history and informs future positioning.

---

### Step 3.1: Factor-Based Risk Decomposition ⏳ **AFTER 2.1**

**Goal:** Explain portfolio volatility and drawdowns via macro shocks.

**Core equation:**
```
Risk Contribution_k = w^T B_k Σ_{f,k} B_k^T w
```

Where:
- **w** = ES portfolio weights (long/short exposure + cash)
- **B_k** = Factor loadings (how much does each indicator expose to factor k?)
- **Σ_{f,k}** = Factor k volatility
- **RC_k** = Risk contribution from factor k

**What this enables:**

1. **Post-mortem drawdown decomposition:**
   - 2008: Growth collapse (GDP shock) + credit tightening (policy shock)
   - 2020: Volatility spike (VIX shock) + policy emergency response
   - 2022: Inflation shock (CPI surprise) → policy tightening (rate shock)

2. **Risk awareness before the shock:**
   - "Current portfolio is 60% exposed to growth shocks, 30% inflation, 10% policy"
   - "If growth shocks spike 2σ, we lose X% of equity"
   - Allows *ex-ante* position sizing

3. **Regime-specific risk:**
   - Inflation regime: Growth shocks are benign, inflation shocks are lethal
   - Tightening regime: Policy shocks dominate
   - Easing regime: Growth shocks matter most

**Implementation outline:**

```cpp
class PortfolioRiskAnalyzer {
public:
    // Decompose portfolio risk by factor
    struct RiskDecomposition {
        std::vector<double> factorRiskContributions;  // % of total risk per factor
        std::vector<std::string> factorLabels;
        double totalPortfolioRisk;
    };

    static RiskDecomposition analyzeRisk(
        const Eigen::VectorXd& weights,              // ES position + cash
        const MacroFactors& factors,
        const CovarianceMatrix& surpriseCov
    );

    // Crisis case studies: what factors drove losses?
    static RiskDecomposition crisesComparison(
        const std::vector<double>& returnsTo2008,   // Historical ES returns
        const std::vector<double>& returnsTo2020,
        const std::vector<double>& returnsTo2022,
        const std::vector<MacroFactors>& factorsOverTime
    );

    // Risk dashboard: time-varying factor exposure
    static std::vector<RiskDecomposition> rollingRiskDecomposition(
        const Eigen::VectorXd& weights,
        const std::vector<MacroFactors>& factorsOverTime,
        int windowMonths = 12
    );
};
```

**Why this matters:**

This is where the system becomes *actionable*. It's not just analytics—it's risk intelligence.

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

After step 3.1, you can say:

> *"On March 2020, ES dropped 10% because markets repriced growth expectations (−2σ GDP shock) while simultaneously experiencing a volatility spike (VIX shock). Our system predicted that exposure ex-ante and sized accordingly."*

After step 4.1:

> *"We monitor factor volatility in real-time and adjust ES position size to target constant risk-adjusted exposure. During the 2022 inflation shock, we automatically reduced size to avoid drawn-down scenarios."*

That's a system that trades firms pay for.

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
