# Implementation Roadmap: Macro Surprise-Driven Risk Framework

## Overview

This roadmap details the evolution from **covariance on levels** (complete) to **macro surprise extraction** (next) to **factor decomposition** (future) to **risk attribution** (future).

---

## Phase 1: Data Infrastructure (COMPLETE ✅)

### Step 1.1: FRED API Integration ✅
- 5 core economic indicators fetched daily
- 20 unit tests, no API keys required

### Step 1.2: VIX / Alpha Vantage Integration ✅
- Daily volatility index from Alpha Vantage
- 17 unit tests, no API keys required

### Step 1.3: Multivariate Covariance (Levels) ✅
- 8×8 covariance matrix on indicator levels
- DataAligner (frequency alignment: daily → monthly)
- CovarianceCalculator with Frobenius norm signal
- 36 unit tests (17 DataAligner + 19 CovarianceCalculator)

**Current deliverable:** `output.txt` contains Frobenius norm (regime volatility signal).

---

## Phase 1 (Continued): Surprise-Driven Analysis (NEXT)

### Step 1.4: Macro Surprise Extraction ⏳ NEXT PRIORITY

**Objective:** Convert indicator levels X_t to surprises ε_t = X_t − E[X_t].

**Why this matters:**
- Prices react to *shocks*, not levels
- CPI 3.1% is bullish if expected 3.5%, bearish if expected 2.8%
- Covariance on surprises isolates information shocks

**Implementation:**

#### File: `src/DataProcessors/SurpriseTransformer.hpp`
```cpp
struct IndicatorSurprise {
    std::vector<double> surprises;      // ε_t (should be mean ≈ 0)
    std::vector<double> rawValues;      // X_t
    std::vector<double> expectations;   // E[X_t]
    std::string dataSource;              // "survey", "ar1", "previous", "none"
    bool isValidated;                    // mean(ε) close to 0?
};

class SurpriseTransformer {
public:
    // Convert levels → surprises for each indicator
    static IndicatorSurprise transformToSurprise(
        const std::vector<double>& levels,
        const std::string& indicator,
        int lookbackMonths = 12
    );

    // Validate zero-mean property
    static bool validateSurprise(const IndicatorSurprise& surprise);

    // Load survey expectations from CSV/JSON (CPI consensus, NFP, etc.)
    static std::vector<double> loadSurveyExpectations(const std::string& indicator);

private:
    // AR(1) forecast: forecast next value based on past 12 months
    static double forecastAR1(const std::vector<double>& historicalValues);
};
```

#### File: `src/DataProcessors/SurpriseTransformer.cpp`

**Key logic:**

```cpp
IndicatorSurprise SurpriseTransformer::transformToSurprise(
    const std::vector<double>& levels,
    const std::string& indicator,
    int lookbackMonths)
{
    IndicatorSurprise result;
    result.rawValues = levels;
    result.surprises.resize(levels.size());
    result.expectations.resize(levels.size());

    for (size_t i = 0; i < levels.size(); i++) {
        // Try survey first, then AR(1), then previous, then zero
        double expected = 0.0;
        std::string source = "none";

        // 1. Check if survey data exists for this indicator at time i
        auto surveyExp = loadSurveyExpectations(indicator);
        if (i < surveyExp.size() && surveyExp[i] != 0) {
            expected = surveyExp[i];
            source = "survey";
        }
        // 2. Otherwise, use AR(1) forecast from previous 12 months
        else if (i >= 12) {
            std::vector<double> history(levels.begin(), levels.begin() + i);
            expected = forecastAR1(history);
            source = "ar1";
        }
        // 3. Or use previous release (for low-frequency data like CPI)
        else if (i > 0) {
            expected = levels[i - 1];
            source = "previous";
        }
        // 4. Else zero (for first release)
        else {
            expected = 0.0;
            source = "none";
        }

        result.expectations[i] = expected;
        result.surprises[i] = levels[i] - expected;
    }

    // Validate zero-mean
    double mean = 0.0;
    for (double s : result.surprises) mean += s;
    mean /= result.surprises.size();
    result.isValidated = (std::abs(mean) < 0.01);  // Allow small bias

    result.dataSource = source;
    return result;
}

double SurpriseTransformer::forecastAR1(const std::vector<double>& history) {
    // Simple AR(1): forecast = α + β * last_value
    // For robustness, use rolling regression on past 12 months
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

#### File: `test/SurpriseTransformerUnitTest.cpp`

**Unit tests (target: 15+ tests):**

```cpp
TEST(SurpriseTransformerTest, ZeroMeanProperty) {
    // Surprises should have mean ≈ 0
    std::vector<double> levels = {100, 102, 101, 103, 99, 104};
    auto surprise = SurpriseTransformer::transformToSurprise(levels, "test");

    double mean = 0.0;
    for (auto s : surprise.surprises) mean += s;
    mean /= surprise.surprises.size();

    EXPECT_NEAR(mean, 0.0, 0.1);
    EXPECT_TRUE(surprise.isValidated);
}

TEST(SurpriseTransformerTest, AR1Forecast) {
    std::vector<double> levels = {100, 101, 102, 103, 104, 105};
    auto surprise = SurpriseTransformer::transformToSurprise(levels, "test");

    // AR(1) should forecast 106 (roughly)
    EXPECT_NEAR(surprise.expectations.back(), 105.5, 1.0);
}

TEST(SurpriseTransformerTest, SurveyExpectation) {
    // If survey says CPI expected 3.0%, actual 3.1%
    // Surprise should be +0.1%
    // (requires mock survey data)
}

TEST(SurpriseTransformerTest, ValidationFails) {
    // Highly biased data should fail validation
    std::vector<double> levels = {1000, 1001, 1002, 1003, 1004};
    auto surprise = SurpriseTransformer::transformToSurprise(levels, "test");
    EXPECT_FALSE(surprise.isValidated);
}
```

**Integration:**
- Update `MultiVarAnalysisWorkflow.cpp` to use `SurpriseTransformer`
- Replace covariance on levels with covariance on surprises
- Store both levels and surprises for debugging

---

### Step 1.5: Macro Factor Decomposition ⏳ FUTURE (after 1.4)

**Objective:** Decompose 8×8 surprise covariance into 3–4 interpretable factors.

**Why this matters:**
- 8×8 covariance has 36 unique entries (overfitting-prone)
- Economic theory suggests 3–4 factors drive everything
- PCA extracts factors automatically; economic labeling makes them interpretable

**Implementation:**

#### File: `src/DataProcessors/MacroFactorModel.hpp`

```cpp
struct MacroFactors {
    std::vector<Eigen::VectorXd> factors;           // Factor time series (K × T)
    Eigen::MatrixXd loadings;                       // B matrix (8 × K)
    std::vector<double> factorVariances;            // Var(factor_k)
    std::vector<std::string> factorLabels;          // e.g., "Growth", "Inflation"
    double cumulativeVarianceExplained;             // % of total variance
    Eigen::MatrixXd residualCovariance;             // Σ_ε (8 × 8)
};

class MacroFactorModel {
public:
    // Decompose surprise covariance using PCA
    static MacroFactors decomposeCovariance(
        const CovarianceMatrix& surpriseCov,
        int numFactors = 3
    );

    // Label factors economically based on loadings
    static std::string labelFactor(
        const Eigen::VectorXd& loading,
        const std::vector<std::string>& indicatorNames
    );

    // Rolling-window factor decomposition (for regime tracking)
    static std::vector<MacroFactors> rollingDecomposition(
        const std::map<std::string, std::vector<double>>& alignedSurprises,
        int windowMonths = 12,
        int numFactors = 3
    );
};
```

#### File: `src/DataProcessors/MacroFactorModel.cpp`

**Key logic (PCA via Eigen):**

```cpp
MacroFactors MacroFactorModel::decomposeCovariance(
    const CovarianceMatrix& surpriseCov,
    int numFactors)
{
    MacroFactors result;

    Eigen::MatrixXd cov = surpriseCov.getMatrix();
    auto indicatorNames = surpriseCov.getIndicatorNames();

    // Compute eigendecomposition
    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> solver(cov);

    Eigen::VectorXd eigenvalues = solver.eigenvalues().reverse();  // Descending
    Eigen::MatrixXd eigenvectors = solver.eigenvectors().rowwise().reverse();

    // Extract top K factors
    Eigen::MatrixXd topEigenvectors = eigenvectors.leftCols(numFactors);
    Eigen::VectorXd topEigenvalues = eigenvalues.head(numFactors);

    // Loadings: B = U * sqrt(Λ)
    result.loadings = topEigenvectors * topEigenvalues.cwiseSqrt().asDiagonal();
    result.factorVariances = topEigenvalues.head(numFactors).reshaped<Eigen::RowMajor>().eval();

    // Label factors
    for (int k = 0; k < numFactors; k++) {
        std::string label = labelFactor(result.loadings.col(k), indicatorNames);
        result.factorLabels.push_back(label);
    }

    // Cumulative variance explained
    double totalVar = eigenvalues.sum();
    result.cumulativeVarianceExplained = topEigenvalues.sum() / totalVar;

    // Residual covariance: Σ_ε = Σ - B B^T
    result.residualCovariance = cov - result.loadings * result.loadings.transpose();

    return result;
}

std::string MacroFactorModel::labelFactor(
    const Eigen::VectorXd& loading,
    const std::vector<std::string>& indicatorNames)
{
    // Heuristic: sort by absolute loading magnitude
    // Growth: high on GDP, unemployment↓, sentiment; low on inflation, policy
    // Inflation: high on CPI; low on unemployment
    // Policy: high on Fed Funds, spreads
    // Volatility: high on VIX

    std::map<std::string, double> loadingMap;
    for (size_t i = 0; i < indicatorNames.size(); i++) {
        loadingMap[indicatorNames[i]] = loading(i);
    }

    // Score each factor type
    double growthScore = 0.0, inflationScore = 0.0, policyScore = 0.0, volScore = 0.0;

    if (loadingMap.count("gdp")) growthScore += loadingMap["gdp"];
    if (loadingMap.count("unemployment")) growthScore -= loadingMap["unemployment"];
    if (loadingMap.count("sentiment")) growthScore += loadingMap["sentiment"];

    if (loadingMap.count("inflation")) inflationScore += loadingMap["inflation"];
    if (loadingMap.count("cpi")) inflationScore += loadingMap["cpi"];

    if (loadingMap.count("fed_funds")) policyScore += loadingMap["fed_funds"];

    if (loadingMap.count("vix")) volScore += loadingMap["vix"];

    // Return label with highest score
    if (std::abs(volScore) > std::max({std::abs(growthScore), std::abs(inflationScore), std::abs(policyScore)})) {
        return "Volatility";
    }
    if (std::abs(inflationScore) > std::max({std::abs(growthScore), std::abs(policyScore)})) {
        return "Inflation";
    }
    if (std::abs(policyScore) > std::abs(growthScore)) {
        return "Policy";
    }
    return "Growth";
}
```

#### File: `test/MacroFactorModelUnitTest.cpp`

**Unit tests (target: 12+ tests):**

```cpp
TEST(MacroFactorModelTest, DecomposeCovariance) {
    // Create mock 8x8 covariance (known structure)
    CovarianceMatrix cov = createMockCov();

    MacroFactors factors = MacroFactorModel::decomposeCovariance(cov, 3);

    // Should have 3 factors
    EXPECT_EQ(factors.loadings.cols(), 3);
    EXPECT_EQ(factors.factorLabels.size(), 3);

    // Variance explained should be >70% for 3 factors
    EXPECT_GT(factors.cumulativeVarianceExplained, 0.7);
}

TEST(MacroFactorModelTest, FactorOrthogonality) {
    MacroFactors factors = MacroFactorModel::decomposeCovariance(cov, 3);

    // Factors should be approximately orthogonal
    for (int i = 0; i < 3; i++) {
        for (int j = i + 1; j < 3; j++) {
            double dot = factors.loadings.col(i).dot(factors.loadings.col(j));
            EXPECT_NEAR(dot, 0.0, 1e-6);
        }
    }
}

TEST(MacroFactorModelTest, FactorLabels) {
    MacroFactors factors = MacroFactorModel::decomposeCovariance(cov, 3);

    // Labels should be one of: Growth, Inflation, Policy, Volatility
    for (const auto& label : factors.factorLabels) {
        EXPECT_TRUE(label == "Growth" || label == "Inflation" ||
                    label == "Policy" || label == "Volatility");
    }
}
```

---

## Phase 2: Risk Attribution (FUTURE)

### Phase 2.5: Portfolio Risk Decomposition ⏳ AFTER 1.5

**Objective:** Explain ES drawdowns using factor exposure.

**Key output:**
```
Risk Contribution_k = w^T B_k Σ_{f,k} B_k^T w
```

**Questions answered:**
- How much of ES risk comes from growth shocks?
- How much from inflation shocks?
- Which factors dominated 2008? 2020? 2022 drawdowns?

**Implementation outline:**
- `src/Analysis/PortfolioRiskAnalyzer.cpp/hpp`
- Compute time-varying risk attribution
- Compare crisis periods (2008, 2020, 2022)
- Generate regime heatmaps

---

## Phase 3: Trading System (FUTURE)

### Phase 3.0: ES Positioning Based on Factor Exposure ⏳ AFTER 2.5

**Objective:** Build ES trading strategy that respects factor structure.

**Key idea:**
- Don't size by raw Frobenius norm
- Size based on factor exposures (growth, inflation, policy)
- Reduce size during high-volatility regimes
- Hedge inflation risk during tightening cycles

---

## Summary: Next Steps

| Step | Focus | Complexity | Impact | Timeline |
|------|-------|-----------|--------|----------|
| **1.4** | Extract surprises (AR1, survey) | Medium | **Huge** (fixes levels problem) | 2–3 days |
| **1.5** | PCA factor decomposition | Medium | **High** (makes covariance interpretable) | 3–4 days |
| **2.5** | Risk attribution (backtesting) | Medium | **High** (explains returns) | 4–5 days |
| **3.0** | Factor-aware trading | High | **Depends on 2.5** | 5–7 days |

**Recommendation:** Do 1.4 → 1.5 → 2.5 in sequence. Each unlocks the next.

---

## Code References

- `src/DataProcessors/CovarianceCalculator.cpp` (existing, will wrap with surprises)
- `src/MultiVarAnalysisWorkflow.cpp` (will orchestrate surprise pipeline)
- `test/` (new: SurpriseTransformerUnitTest, MacroFactorModelUnitTest)

---

## Success Criteria

- [ ] SurpriseTransformer validates surprises have mean ≈ 0
- [ ] MacroFactorModel reduces 8×8 → 3-4 factors with >70% variance explained
- [ ] PortfolioRiskAnalyzer explains past drawdowns
- [ ] All unit tests pass (20+ new tests)
- [ ] README remains public-facing; ROADMAP contains implementation detail
