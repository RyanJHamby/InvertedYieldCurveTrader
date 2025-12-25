//
//  PortfolioRiskAnalyzerUnitTest.cpp
//  InvertedYieldCurveTrader
//
//  Unit tests for PortfolioRiskAnalyzer (risk attribution by macro factor)
//
//  Created by Ryan Hamby on 12/25/25.
//

#include <gtest/gtest.h>
#include "../src/DataProcessors/PortfolioRiskAnalyzer.hpp"
#include "../src/DataProcessors/CovarianceCalculator.hpp"
#include <cmath>
#include <numeric>

class PortfolioRiskAnalyzerTest : public ::testing::Test {
protected:
    // Create mock factor model with 3 factors and 8 indicators
    static MacroFactors createMockFactorModel() {
        MacroFactors factors;
        factors.numFactors = 3;
        factors.factorLabels = {"Growth", "Inflation", "Volatility"};
        factors.labelConfidences = {0.85, 0.78, 0.72};
        factors.factorVariances = {0.40, 0.25, 0.15};  // Descending variances
        factors.cumulativeVarianceExplained = 0.80;
        factors.indicatorNames = {
            "fed_funds", "unemployment", "consumer_sentiment", "gdp",
            "inflation", "treasury_10y", "vix", "move"
        };

        // Create loadings matrix B (8 × 3)
        factors.loadings = Eigen::MatrixXd::Zero(8, 3);

        // Growth factor: positive on unemployment (inverse), sentiment, GDP
        factors.loadings(1, 0) = -0.35;  // unemployment (negative = growth)
        factors.loadings(2, 0) = 0.40;   // sentiment
        factors.loadings(3, 0) = 0.38;   // GDP

        // Inflation factor: positive on inflation, CPI
        factors.loadings(4, 1) = 0.50;   // inflation
        factors.loadings(5, 1) = 0.42;   // treasury (bond response)

        // Volatility factor: positive on VIX, MOVE
        factors.loadings(6, 2) = 0.60;   // VIX
        factors.loadings(7, 2) = 0.55;   // MOVE

        // Residual covariance
        factors.residualCovariance = Eigen::MatrixXd::Zero(8, 8);

        return factors;
    }

    // Create mock portfolio sensitivities (β)
    static Eigen::VectorXd createPortfolioSensitivities() {
        Eigen::VectorXd beta(8);
        // ES-like exposure: sensitive to growth, inflation policy, volatility
        beta(0) = 0.3;   // fed_funds: positive (tightening bad for ES)
        beta(1) = -0.5;  // unemployment: negative (falling is good)
        beta(2) = 0.4;   // sentiment: positive
        beta(3) = 0.6;   // GDP: positive (strong growth is good)
        beta(4) = -0.2;  // inflation: negative (high inflation hurts valuations)
        beta(5) = -0.1;  // treasury: slightly negative
        beta(6) = -0.8;  // VIX: strong negative (high VIX bad)
        beta(7) = -0.7;  // MOVE: negative
        return beta;
    }
};

// ===== Basic Risk Attribution Tests =====

TEST_F(PortfolioRiskAnalyzerTest, AnalyzeRiskBasic) {
    auto factors = createMockFactorModel();
    auto beta = createPortfolioSensitivities();

    RiskDecomposition risk = PortfolioRiskAnalyzer::analyzeRisk(beta, factors);

    // Verify structure
    EXPECT_EQ(risk.numFactors, 3);
    EXPECT_EQ(risk.factorSensitivities.size(), 3);
    EXPECT_EQ(risk.factorRiskContributions.size(), 3);
    EXPECT_EQ(risk.componentContributions.size(), 3);
    EXPECT_EQ(risk.factorLabels.size(), 3);
}

TEST_F(PortfolioRiskAnalyzerTest, AnalyzeRiskPositive) {
    auto factors = createMockFactorModel();
    auto beta = createPortfolioSensitivities();

    RiskDecomposition risk = PortfolioRiskAnalyzer::analyzeRisk(beta, factors);

    // Risk should be positive
    EXPECT_GT(risk.totalRisk, 0.0);
    EXPECT_GT(risk.totalVariance, 0.0);

    // All risk contributions should be non-negative
    double sumRC = 0.0;
    for (double rc : risk.factorRiskContributions) {
        EXPECT_GE(rc, 0.0);
        sumRC += rc;
    }

    // Sum of risk contributions ≈ total variance
    EXPECT_NEAR(sumRC, risk.totalVariance, 1e-8);
}

TEST_F(PortfolioRiskAnalyzerTest, AnalyzeRiskComponentContributions) {
    auto factors = createMockFactorModel();
    auto beta = createPortfolioSensitivities();

    RiskDecomposition risk = PortfolioRiskAnalyzer::analyzeRisk(beta, factors);

    // Component contributions should sum to 1
    double sumComponents = 0.0;
    for (double comp : risk.componentContributions) {
        EXPECT_GE(comp, 0.0);
        EXPECT_LE(comp, 1.0);
        sumComponents += comp;
    }
    EXPECT_NEAR(sumComponents, 1.0, 1e-8);
}

TEST_F(PortfolioRiskAnalyzerTest, AnalyzeRiskFactorSensitivities) {
    auto factors = createMockFactorModel();
    auto beta = createPortfolioSensitivities();

    RiskDecomposition risk = PortfolioRiskAnalyzer::analyzeRisk(beta, factors);

    // Factor sensitivities should be non-zero
    int nonZeroSensitivities = 0;
    for (double gamma : risk.factorSensitivities) {
        if (std::abs(gamma) > 1e-10) {
            nonZeroSensitivities++;
        }
    }
    EXPECT_GT(nonZeroSensitivities, 0);
}

// ===== Scenario Analysis Tests =====

TEST_F(PortfolioRiskAnalyzerTest, ScenarioShockImpact) {
    auto factors = createMockFactorModel();
    auto beta = createPortfolioSensitivities();

    // Scenario: Growth shock (factor 1) down 2 standard deviations
    Eigen::VectorXd shock(3);
    shock(0) = -2.0 * std::sqrt(factors.factorVariances[0]);  // Growth shock
    shock(1) = 0.0;
    shock(2) = 0.0;

    double impact = PortfolioRiskAnalyzer::scenarioShockImpact(beta, shock, factors);

    // If ES is growth-sensitive (positive loading), negative growth shock = loss
    // beta(3)=0.6 for GDP × growth loading ≈ 0.38 → large negative impact expected
    EXPECT_LT(impact, 0.0);
}

TEST_F(PortfolioRiskAnalyzerTest, ScenarioVolatilityShock) {
    auto factors = createMockFactorModel();
    auto beta = createPortfolioSensitivities();

    // Scenario: Volatility shock (factor 3) up 1 std dev
    Eigen::VectorXd shock(3);
    shock(0) = 0.0;
    shock(1) = 0.0;
    shock(2) = 1.0 * std::sqrt(factors.factorVariances[2]);

    double impact = PortfolioRiskAnalyzer::scenarioShockImpact(beta, shock, factors);

    // ES is volatility-negative (beta for VIX/MOVE < 0)
    // Positive volatility shock = loss
    EXPECT_LT(impact, 0.0);
}

TEST_F(PortfolioRiskAnalyzerTest, ScenarioZeroShock) {
    auto factors = createMockFactorModel();
    auto beta = createPortfolioSensitivities();

    // Zero shock → zero impact
    Eigen::VectorXd shock = Eigen::VectorXd::Zero(3);
    double impact = PortfolioRiskAnalyzer::scenarioShockImpact(beta, shock, factors);

    EXPECT_NEAR(impact, 0.0, 1e-10);
}

TEST_F(PortfolioRiskAnalyzerTest, ScenarioOppositeShocks) {
    auto factors = createMockFactorModel();
    auto beta = createPortfolioSensitivities();

    // Scenario 1: Growth +2σ
    Eigen::VectorXd shock1(3);
    shock1(0) = 2.0 * std::sqrt(factors.factorVariances[0]);
    shock1(1) = 0.0;
    shock1(2) = 0.0;
    double impact1 = PortfolioRiskAnalyzer::scenarioShockImpact(beta, shock1, factors);

    // Scenario 2: Growth -2σ
    Eigen::VectorXd shock2(3);
    shock2(0) = -2.0 * std::sqrt(factors.factorVariances[0]);
    shock2(1) = 0.0;
    shock2(2) = 0.0;
    double impact2 = PortfolioRiskAnalyzer::scenarioShockImpact(beta, shock2, factors);

    // Impacts should be equal in magnitude, opposite in sign
    EXPECT_NEAR(impact1, -impact2, 1e-10);
}

// ===== Historical Drawdown Tests =====

TEST_F(PortfolioRiskAnalyzerTest, ExplainHistoricalDrawdown) {
    // Simulate a crisis: portfolio drops 20% while factors move
    std::vector<double> returns = {-0.05, -0.08, -0.04, -0.03};
    std::vector<std::vector<double>> shocks = {
        {-0.1, 0.02, 0.15},   // Growth shock down, inflation up, volatility up
        {-0.08, 0.03, 0.18},
        {-0.06, 0.02, 0.12},
        {-0.04, 0.01, 0.08}
    };
    std::vector<std::string> labels = {"Growth", "Inflation", "Volatility"};

    RiskDecomposition decomp = PortfolioRiskAnalyzer::explainHistoricalDrawdown(
        returns, shocks, labels);

    // Should recover sensitivities and explain loss
    EXPECT_EQ(decomp.numFactors, 3);
    EXPECT_EQ(decomp.factorLabels.size(), 3);

    // R² should be positive (some variance explained)
    EXPECT_GT(decomp.varianceExplained, 0.0);
}

TEST_F(PortfolioRiskAnalyzerTest, ExplainDrawdownLabelPreservation) {
    std::vector<double> returns = {-0.01, -0.02, -0.01, -0.01};
    std::vector<std::vector<double>> shocks = {
        {-0.1, 0.02, 0.15},
        {-0.08, 0.03, 0.18},
        {-0.06, 0.02, 0.12},
        {-0.04, 0.01, 0.08}
    };
    std::vector<std::string> labels = {"Growth", "Inflation", "Volatility"};

    RiskDecomposition decomp = PortfolioRiskAnalyzer::explainHistoricalDrawdown(
        returns, shocks, labels);

    // Factor labels should be preserved
    EXPECT_EQ(decomp.factorLabels[0], "Growth");
    EXPECT_EQ(decomp.factorLabels[1], "Inflation");
    EXPECT_EQ(decomp.factorLabels[2], "Volatility");
}

// ===== Rolling Risk Decomposition Tests =====

TEST_F(PortfolioRiskAnalyzerTest, RollingRiskDecomposition) {
    auto factors1 = createMockFactorModel();
    auto factors2 = createMockFactorModel();
    factors2.factorVariances = {0.35, 0.28, 0.20};  // Different volatilities

    std::vector<MacroFactors> factorsOverTime = {factors1, factors2};
    auto beta = createPortfolioSensitivities();

    std::vector<RiskDecomposition> rolling =
        PortfolioRiskAnalyzer::rollingRiskDecomposition(beta, factorsOverTime, 12);

    // Should have results for both windows
    EXPECT_EQ(rolling.size(), 2);

    // Each should have valid risk decomposition
    for (const auto& decomp : rolling) {
        EXPECT_EQ(decomp.numFactors, 3);
        EXPECT_GT(decomp.totalVariance, 0.0);
    }
}

TEST_F(PortfolioRiskAnalyzerTest, RollingRiskDecompositionVariability) {
    auto factors1 = createMockFactorModel();
    auto factors2 = createMockFactorModel();
    factors2.factorVariances = {0.50, 0.30, 0.10};  // Much different

    std::vector<MacroFactors> factorsOverTime = {factors1, factors2};
    auto beta = createPortfolioSensitivities();

    std::vector<RiskDecomposition> rolling =
        PortfolioRiskAnalyzer::rollingRiskDecomposition(beta, factorsOverTime, 12);

    // Risk profiles should be different between windows
    EXPECT_NE(rolling[0].totalVariance, rolling[1].totalVariance);
}

// ===== Risk Budgeting Tests =====

TEST_F(PortfolioRiskAnalyzerTest, RiskBudgetingAllocation) {
    auto factors = createMockFactorModel();
    double targetRisk = 0.15;  // 15% volatility target

    RiskDecomposition allocation =
        PortfolioRiskAnalyzer::riskBudgetingAllocation(targetRisk, factors);

    // Should achieve target risk
    EXPECT_NEAR(allocation.totalRisk, targetRisk, 0.01);

    // Components should sum to 1
    double sumComponents = 0.0;
    for (double comp : allocation.componentContributions) {
        sumComponents += comp;
    }
    EXPECT_NEAR(sumComponents, 1.0, 1e-8);
}

TEST_F(PortfolioRiskAnalyzerTest, RiskBudgetingDifferentTargets) {
    auto factors = createMockFactorModel();

    // Allocate to different risk targets
    double target1 = 0.10;
    double target2 = 0.20;

    RiskDecomposition alloc1 =
        PortfolioRiskAnalyzer::riskBudgetingAllocation(target1, factors);
    RiskDecomposition alloc2 =
        PortfolioRiskAnalyzer::riskBudgetingAllocation(target2, factors);

    // Higher target should have higher risk
    EXPECT_LT(alloc1.totalRisk, alloc2.totalRisk);

    // But similar component contributions (same allocation strategy)
    EXPECT_NEAR(alloc1.componentContributions[0],
               alloc2.componentContributions[0], 0.1);
}

// ===== Error Handling Tests =====

TEST_F(PortfolioRiskAnalyzerTest, AnalyzeRiskMismatchedSizes) {
    auto factors = createMockFactorModel();
    Eigen::VectorXd beta(7);  // Wrong size (8 expected)
    beta.setRandom();

    EXPECT_THROW(
        PortfolioRiskAnalyzer::analyzeRisk(beta, factors),
        std::invalid_argument
    );
}

TEST_F(PortfolioRiskAnalyzerTest, ScenarioShockWrongSize) {
    auto factors = createMockFactorModel();
    auto beta = createPortfolioSensitivities();
    Eigen::VectorXd shock(2);  // Wrong size (3 expected)
    shock.setRandom();

    EXPECT_THROW(
        PortfolioRiskAnalyzer::scenarioShockImpact(beta, shock, factors),
        std::invalid_argument
    );
}

TEST_F(PortfolioRiskAnalyzerTest, ExplainDrawdownEmptyReturns) {
    std::vector<double> returns;  // Empty
    std::vector<std::vector<double>> shocks = {{0.1, 0.2, 0.3}};
    std::vector<std::string> labels = {"Growth", "Inflation", "Volatility"};

    EXPECT_THROW(
        PortfolioRiskAnalyzer::explainHistoricalDrawdown(returns, shocks, labels),
        std::invalid_argument
    );
}

TEST_F(PortfolioRiskAnalyzerTest, RiskBudgetingNegativeTarget) {
    auto factors = createMockFactorModel();

    EXPECT_THROW(
        PortfolioRiskAnalyzer::riskBudgetingAllocation(-0.15, factors),
        std::invalid_argument
    );
}

TEST_F(PortfolioRiskAnalyzerTest, RollingEmptyFactors) {
    auto beta = createPortfolioSensitivities();
    std::vector<MacroFactors> empty;

    EXPECT_THROW(
        PortfolioRiskAnalyzer::rollingRiskDecomposition(beta, empty, 12),
        std::invalid_argument
    );
}

// Run tests
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
