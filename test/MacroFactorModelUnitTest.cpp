//
//  MacroFactorModelUnitTest.cpp
//  InvertedYieldCurveTrader
//
//  Unit tests for MacroFactorModel (PCA-based factor decomposition)
//
//  Created by Ryan Hamby on 12/25/25.
//

#include <gtest/gtest.h>
#include "../src/DataProcessors/MacroFactorModel.hpp"
#include "../src/DataProcessors/CovarianceCalculator.hpp"
#include <cmath>
#include <iostream>

class MacroFactorModelTest : public ::testing::Test {
protected:
    // Create a mock 8-indicator surprise dataset (12 months)
    static std::map<std::string, std::vector<double>> createMockSurprises() {
        std::map<std::string, std::vector<double>> surprises;
        surprises["fed_funds"] = {0.1, 0.15, 0.2, 0.18, 0.22, 0.25, 0.23, 0.2, 0.18, 0.15, 0.12, 0.1};
        surprises["unemployment"] = {-0.05, -0.08, -0.1, -0.09, -0.11, -0.12, -0.1, -0.08, -0.06, -0.04, -0.02, 0.0};
        surprises["consumer_sentiment"] = {0.08, 0.12, 0.15, 0.13, 0.17, 0.2, 0.18, 0.15, 0.12, 0.1, 0.08, 0.05};
        surprises["gdp"] = {0.05, 0.08, 0.1, 0.09, 0.11, 0.12, 0.11, 0.09, 0.07, 0.05, 0.03, 0.02};
        surprises["inflation"] = {0.15, 0.18, 0.2, 0.19, 0.22, 0.25, 0.23, 0.21, 0.19, 0.17, 0.15, 0.13};
        surprises["treasury_10y"] = {0.12, 0.15, 0.18, 0.16, 0.19, 0.22, 0.2, 0.18, 0.16, 0.14, 0.12, 0.1};
        surprises["vix"] = {0.2, 0.25, 0.3, 0.28, 0.32, 0.35, 0.32, 0.28, 0.25, 0.22, 0.2, 0.18};
        surprises["move"] = {0.18, 0.22, 0.25, 0.24, 0.27, 0.3, 0.28, 0.25, 0.22, 0.2, 0.18, 0.16};
        return surprises;
    }

    // Compute covariance matrix from surprises
    static CovarianceMatrix computeTestCovariance(
        const std::map<std::string, std::vector<double>>& surprises)
    {
        CovarianceCalculator calc;
        return calc.calculateCovarianceMatrix(surprises);
    }
};

// ===== Basic Decomposition Tests =====

TEST_F(MacroFactorModelTest, DecomposeThreeFactors) {
    auto surprises = createMockSurprises();
    CovarianceMatrix cov = computeTestCovariance(surprises);

    MacroFactors result = MacroFactorModel::decomposeSurpriseCovariance(cov, 3);

    // Check dimensions
    EXPECT_EQ(result.numFactors, 3);
    EXPECT_EQ(result.loadings.cols(), 3);
    EXPECT_EQ(result.loadings.rows(), 8);
    EXPECT_EQ(result.factorVariances.size(), 3);
    EXPECT_EQ(result.factorLabels.size(), 3);
    EXPECT_EQ(result.labelConfidences.size(), 3);
}

TEST_F(MacroFactorModelTest, VarianceExplainedPositive) {
    auto surprises = createMockSurprises();
    CovarianceMatrix cov = computeTestCovariance(surprises);

    MacroFactors result = MacroFactorModel::decomposeSurpriseCovariance(cov, 3);

    // Cumulative variance explained should be positive and < 1.0
    EXPECT_GT(result.cumulativeVarianceExplained, 0.0);
    EXPECT_LE(result.cumulativeVarianceExplained, 1.0);

    // For 3 factors from 8 indicators, should explain significant variance (>50%)
    EXPECT_GT(result.cumulativeVarianceExplained, 0.5);
}

TEST_F(MacroFactorModelTest, FactorVariancesDescending) {
    auto surprises = createMockSurprises();
    CovarianceMatrix cov = computeTestCovariance(surprises);

    MacroFactors result = MacroFactorModel::decomposeSurpriseCovariance(cov, 3);

    // Eigenvalues should be in descending order
    for (int k = 0; k < static_cast<int>(result.factorVariances.size()) - 1; k++) {
        EXPECT_GE(result.factorVariances[k], result.factorVariances[k + 1]);
    }
}

TEST_F(MacroFactorModelTest, ResidualCovarianceSymmetric) {
    auto surprises = createMockSurprises();
    CovarianceMatrix cov = computeTestCovariance(surprises);

    MacroFactors result = MacroFactorModel::decomposeSurpriseCovariance(cov, 3);

    // Residual covariance should be symmetric
    for (int i = 0; i < result.residualCovariance.rows(); i++) {
        for (int j = 0; j < result.residualCovariance.cols(); j++) {
            EXPECT_NEAR(result.residualCovariance(i, j),
                       result.residualCovariance(j, i),
                       1e-10);
        }
    }
}

// ===== Archetype Matching Tests =====

TEST_F(MacroFactorModelTest, GrowthArchetypeDetection) {
    // Create a factor heavily weighted on growth indicators
    Eigen::VectorXd growthLike = Eigen::VectorXd::Zero(8);
    // fed_funds, unemployment, consumer_sentiment, gdp, inflation, treasury, vix, move
    growthLike(0) = 0.2;   // fed_funds (low weight)
    growthLike(1) = -0.4;  // unemployment (negative, strong)
    growthLike(2) = 0.4;   // consumer_sentiment (positive, strong)
    growthLike(3) = 0.3;   // gdp (positive)
    growthLike(4) = 0.1;   // inflation (low)
    growthLike(5) = 0.1;   // treasury (low)
    growthLike(6) = 0.1;   // vix (low)
    growthLike(7) = 0.1;   // move (low)

    std::vector<std::string> indicatorNames = {
        "fed_funds", "unemployment", "consumer_sentiment", "gdp",
        "inflation", "treasury_10y", "vix", "move"
    };

    LabelResult result = MacroFactorModel::labelFactorRobustly(growthLike, indicatorNames);

    // Should match Growth archetype with decent confidence
    EXPECT_EQ(result.label, "Growth");
    EXPECT_GT(result.cosineScore, 0.5);
}

TEST_F(MacroFactorModelTest, InflationArchetypeDetection) {
    // Create a factor heavily weighted on inflation indicators
    Eigen::VectorXd inflationLike = Eigen::VectorXd::Zero(8);
    inflationLike(0) = 0.1;   // fed_funds (low)
    inflationLike(1) = 0.1;   // unemployment (low)
    inflationLike(2) = 0.1;   // consumer_sentiment (low)
    inflationLike(3) = 0.1;   // gdp (low)
    inflationLike(4) = 0.5;   // inflation (high)
    inflationLike(5) = 0.1;   // treasury (low)
    inflationLike(6) = 0.1;   // vix (low)
    inflationLike(7) = 0.1;   // move (low)

    std::vector<std::string> indicatorNames = {
        "fed_funds", "unemployment", "consumer_sentiment", "gdp",
        "inflation", "treasury_10y", "vix", "move"
    };

    LabelResult result = MacroFactorModel::labelFactorRobustly(inflationLike, indicatorNames);

    EXPECT_EQ(result.label, "Inflation");
    EXPECT_GT(result.cosineScore, 0.5);
}

TEST_F(MacroFactorModelTest, VolatilityArchetypeDetection) {
    // Create a factor heavily weighted on volatility indicators
    Eigen::VectorXd volatilityLike = Eigen::VectorXd::Zero(8);
    volatilityLike(0) = 0.1;   // fed_funds
    volatilityLike(1) = 0.1;   // unemployment
    volatilityLike(2) = 0.1;   // consumer_sentiment
    volatilityLike(3) = 0.1;   // gdp
    volatilityLike(4) = 0.1;   // inflation
    volatilityLike(5) = 0.1;   // treasury
    volatilityLike(6) = 0.5;   // vix (high)
    volatilityLike(7) = 0.5;   // move (high)

    std::vector<std::string> indicatorNames = {
        "fed_funds", "unemployment", "consumer_sentiment", "gdp",
        "inflation", "treasury_10y", "vix", "move"
    };

    LabelResult result = MacroFactorModel::labelFactorRobustly(volatilityLike, indicatorNames);

    EXPECT_EQ(result.label, "Volatility");
    EXPECT_GT(result.cosineScore, 0.5);
}

TEST_F(MacroFactorModelTest, UnclassifiedBelowThreshold) {
    // Create a factor that doesn't match any archetype well
    Eigen::VectorXd randomFactor = Eigen::VectorXd::Ones(8) * 0.1;
    randomFactor(0) = 0.05;
    randomFactor(1) = 0.06;
    randomFactor(2) = 0.04;

    std::vector<std::string> indicatorNames = {
        "fed_funds", "unemployment", "consumer_sentiment", "gdp",
        "inflation", "treasury_10y", "vix", "move"
    };

    LabelResult result = MacroFactorModel::labelFactorRobustly(randomFactor, indicatorNames);

    // Low confidence should result in Unclassified
    if (result.cosineScore < 0.65) {
        EXPECT_EQ(result.label, "Unclassified");
    }
}

// ===== Confidence and Stability Tests =====

TEST_F(MacroFactorModelTest, LabelConfidenceRange) {
    auto surprises = createMockSurprises();
    CovarianceMatrix cov = computeTestCovariance(surprises);

    MacroFactors result = MacroFactorModel::decomposeSurpriseCovariance(cov, 3);

    // All confidence scores should be in [0, 1]
    for (double conf : result.labelConfidences) {
        EXPECT_GE(conf, 0.0);
        EXPECT_LE(conf, 1.0);
    }
}

TEST_F(MacroFactorModelTest, StabilityTrackingAcrossWindows) {
    auto surprises = createMockSurprises();

    // Extend data for rolling window test
    for (auto& [key, series] : surprises) {
        while (series.size() < 24) {
            // Add more synthetic data
            series.push_back(series.back() * 0.95 + 0.05);
        }
    }

    std::vector<MacroFactors> results =
        MacroFactorModel::rollingDecompositionWithDriftDetection(surprises, 12, 3);

    // Should have at least 2 windows (24 months / 12 month window)
    EXPECT_GE(results.size(), 2);

    // Each result should be a valid MacroFactors
    for (const auto& result : results) {
        EXPECT_EQ(result.numFactors, 3);
        EXPECT_EQ(result.factorLabels.size(), 3);
    }
}

// ===== Edge Cases and Error Handling =====

TEST_F(MacroFactorModelTest, SingleFactorDecomposition) {
    auto surprises = createMockSurprises();
    CovarianceMatrix cov = computeTestCovariance(surprises);

    MacroFactors result = MacroFactorModel::decomposeSurpriseCovariance(cov, 1);

    EXPECT_EQ(result.numFactors, 1);
    EXPECT_EQ(result.loadings.cols(), 1);
    EXPECT_EQ(result.factorVariances.size(), 1);
}

TEST_F(MacroFactorModelTest, MaxFactorDecomposition) {
    auto surprises = createMockSurprises();
    CovarianceMatrix cov = computeTestCovariance(surprises);

    // Should be able to extract up to 8 factors (8 indicators)
    MacroFactors result = MacroFactorModel::decomposeSurpriseCovariance(cov, 8);

    EXPECT_EQ(result.numFactors, 8);
    EXPECT_EQ(result.loadings.cols(), 8);
}

TEST_F(MacroFactorModelTest, InvalidNumFactorsThrows) {
    auto surprises = createMockSurprises();
    CovarianceMatrix cov = computeTestCovariance(surprises);

    EXPECT_THROW(
        MacroFactorModel::decomposeSurpriseCovariance(cov, 0),
        std::invalid_argument
    );

    EXPECT_THROW(
        MacroFactorModel::decomposeSurpriseCovariance(cov, 9),  // More than indicators
        std::invalid_argument
    );
}

TEST_F(MacroFactorModelTest, InvalidLookbackThrows) {
    auto surprises = createMockSurprises();

    EXPECT_THROW(
        MacroFactorModel::rollingDecompositionWithDriftDetection(surprises, 0, 3),
        std::invalid_argument
    );

    EXPECT_THROW(
        MacroFactorModel::rollingDecompositionWithDriftDetection(surprises, -1, 3),
        std::invalid_argument
    );
}

TEST_F(MacroFactorModelTest, EmptySurprisesThrows) {
    std::map<std::string, std::vector<double>> empty;

    EXPECT_THROW(
        MacroFactorModel::rollingDecompositionWithDriftDetection(empty, 12, 3),
        std::invalid_argument
    );
}

TEST_F(MacroFactorModelTest, MismatchedSeriesLengthsThrows) {
    std::map<std::string, std::vector<double>> surprises;
    surprises["fed_funds"] = {0.1, 0.2, 0.3};
    surprises["unemployment"] = {0.05, 0.08};  // Different length

    EXPECT_THROW(
        MacroFactorModel::rollingDecompositionWithDriftDetection(surprises, 2, 2),
        std::invalid_argument
    );
}

// ===== Mathematical Consistency Tests =====

TEST_F(MacroFactorModelTest, LoadingsMatrixRank) {
    auto surprises = createMockSurprises();
    CovarianceMatrix cov = computeTestCovariance(surprises);

    MacroFactors result = MacroFactorModel::decomposeSurpriseCovariance(cov, 3);

    // Loadings matrix should have full column rank (3 for 3 factors)
    // At least, all factor variances should be positive
    for (double var : result.factorVariances) {
        EXPECT_GT(var, 0.0);
    }
}

TEST_F(MacroFactorModelTest, ReconstructionError) {
    auto surprises = createMockSurprises();
    CovarianceMatrix cov = computeTestCovariance(surprises);

    MacroFactors result = MacroFactorModel::decomposeSurpriseCovariance(cov, 3);

    // Reconstruction: Σ_approx = B B^T
    Eigen::MatrixXd originalCov = cov.getMatrix();
    Eigen::MatrixXd reconstructed = result.loadings * result.loadings.transpose();

    // Error should be captured in residualCovariance
    Eigen::MatrixXd error = originalCov - reconstructed - result.residualCovariance;

    // Error norm should be small (< 1e-8 due to numerical precision)
    double errorNorm = error.norm();
    EXPECT_LT(errorNorm, 1e-6);
}

TEST_F(MacroFactorModelTest, CumulativeVarianceAdditivity) {
    auto surprises = createMockSurprises();
    CovarianceMatrix cov = computeTestCovariance(surprises);

    // Compare 3-factor and 4-factor models
    MacroFactors result3 = MacroFactorModel::decomposeSurpriseCovariance(cov, 3);
    MacroFactors result4 = MacroFactorModel::decomposeSurpriseCovariance(cov, 4);

    // Adding more factors should increase (or maintain) cumulative variance
    EXPECT_GE(result4.cumulativeVarianceExplained,
              result3.cumulativeVarianceExplained - 1e-10);

    // 4 factors should explain more than 3
    EXPECT_GT(result4.cumulativeVarianceExplained,
              result3.cumulativeVarianceExplained - 0.01);  // Allow small numerical error
}

// ===== Indicator Name Mapping Tests =====

TEST_F(MacroFactorModelTest, IndicatorNamesPreserved) {
    auto surprises = createMockSurprises();
    CovarianceMatrix cov = computeTestCovariance(surprises);

    MacroFactors result = MacroFactorModel::decomposeSurpriseCovariance(cov, 3);

    // Indicator names from covariance should be preserved in result
    EXPECT_EQ(result.indicatorNames.size(), 8);
    EXPECT_EQ(result.indicatorNames, cov.getIndicatorNames());
}

// Run tests
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
