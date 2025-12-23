//
//  CovarianceCalculatorUnitTest.cpp
//  InvertedYieldCurveTrader
//
//  Unit tests for CovarianceCalculator 8x8 matrix (no API keys required)
//
//  Created by Ryan Hamby on 12/23/25.
//

#include <gtest/gtest.h>
#include "../src/DataProcessors/CovarianceCalculator.hpp"
#include <cmath>
#include <map>
#include <vector>

class CovarianceCalculatorUnitTest : public ::testing::Test {
protected:
    CovarianceCalculator calculator;

    // Create simple 12-month synthetic data
    static std::vector<double> createLinearData(double start, double step) {
        std::vector<double> data;
        for (int i = 11; i >= 0; i--) {  // Most recent first
            data.push_back(start + i * step);
        }
        return data;
    }

    // Create perfectly correlated data (r=1)
    static std::vector<double> createPerfectlyCorrelatedData(const std::vector<double>& reference) {
        return reference;  // Perfect correlation
    }

    // Create inverse correlated data (r=-1)
    static std::vector<double> createInverseCorrelatedData(const std::vector<double>& reference) {
        std::vector<double> inverse;
        for (const auto& val : reference) {
            inverse.push_back(-val);
        }
        return inverse;
    }

    // Create uncorrelated data
    static std::vector<double> createUncorrelatedData() {
        return {10.0, 11.0, 9.0, 12.0, 10.5, 8.0, 13.0, 9.5, 11.5, 10.0, 12.5, 8.5};
    }
};

// ===== CovarianceMatrix Tests =====

TEST_F(CovarianceCalculatorUnitTest, CovarianceMatrix_Constructor) {
    Eigen::MatrixXd mat(2, 2);
    mat << 1.0, 0.5, 0.5, 2.0;
    std::vector<std::string> names = {"var1", "var2"};

    CovarianceMatrix covMat(mat, names);

    EXPECT_EQ(covMat.getIndicatorNames().size(), 2);
    EXPECT_EQ(covMat.getIndicatorNames()[0], "var1");
}

TEST_F(CovarianceCalculatorUnitTest, CovarianceMatrix_GetCovariance) {
    Eigen::MatrixXd mat(2, 2);
    mat << 1.0, 0.5, 0.5, 2.0;
    std::vector<std::string> names = {"var1", "var2"};

    CovarianceMatrix covMat(mat, names);

    EXPECT_DOUBLE_EQ(covMat.getCovariance("var1", "var2"), 0.5);
    EXPECT_DOUBLE_EQ(covMat.getCovariance("var2", "var1"), 0.5);
}

TEST_F(CovarianceCalculatorUnitTest, CovarianceMatrix_GetCovarianceNotFound) {
    Eigen::MatrixXd mat(2, 2);
    mat << 1.0, 0.5, 0.5, 2.0;
    std::vector<std::string> names = {"var1", "var2"};

    CovarianceMatrix covMat(mat, names);

    EXPECT_THROW({
        covMat.getCovariance("var3", "var1");
    }, std::out_of_range);
}

TEST_F(CovarianceCalculatorUnitTest, CovarianceMatrix_FrobeniusNorm) {
    Eigen::MatrixXd mat(2, 2);
    mat << 3.0, 0.0, 0.0, 4.0;
    std::vector<std::string> names = {"var1", "var2"};

    CovarianceMatrix covMat(mat, names);

    // Frobenius norm = sqrt(3^2 + 0^2 + 0^2 + 4^2) = sqrt(9 + 16) = 5
    EXPECT_DOUBLE_EQ(covMat.getFrobeniusNorm(), 5.0);
}

TEST_F(CovarianceCalculatorUnitTest, CovarianceMatrix_GetMatrix) {
    Eigen::MatrixXd mat(2, 2);
    mat << 1.0, 0.5, 0.5, 2.0;
    std::vector<std::string> names = {"var1", "var2"};

    CovarianceMatrix covMat(mat, names);
    Eigen::MatrixXd retrieved = covMat.getMatrix();

    EXPECT_DOUBLE_EQ(retrieved(0, 0), 1.0);
    EXPECT_DOUBLE_EQ(retrieved(0, 1), 0.5);
}

// ===== Pairwise Covariance Tests (Backward Compatibility) =====

TEST_F(CovarianceCalculatorUnitTest, Pairwise_PerfectCorrelation) {
    // Two identical vectors should have cov = var
    std::vector<double> data = createLinearData(10.0, 1.0);
    double mean = 15.5;  // Mean of 10,11,12,...,21

    double cov = calculator.calculateCovarianceWithInvertedYield(data, data, mean, mean);

    // Variance of data
    double variance = 0.0;
    for (const auto& val : data) {
        variance += (val - mean) * (val - mean);
    }
    variance /= (data.size() - 1);

    EXPECT_NEAR(cov, variance, 1e-10);
}

TEST_F(CovarianceCalculatorUnitTest, Pairwise_ZeroCovarianceConstant) {
    std::vector<double> data1 = createLinearData(10.0, 1.0);
    std::vector<double> data2(12, 15.0);  // Constant

    double mean1 = 15.5;
    double mean2 = 15.0;

    double cov = calculator.calculateCovarianceWithInvertedYield(data1, data2, mean1, mean2);

    // Covariance with constant should be 0
    EXPECT_NEAR(cov, 0.0, 1e-10);
}

TEST_F(CovarianceCalculatorUnitTest, Pairwise_MismatchedSizesThrows) {
    std::vector<double> data1 = createLinearData(10.0, 1.0);
    std::vector<double> data2(10, 15.0);

    EXPECT_THROW({
        calculator.calculateCovarianceWithInvertedYield(data1, data2, 15.5, 15.0);
    }, std::invalid_argument);
}

// ===== 8x8 Covariance Matrix Tests =====

TEST_F(CovarianceCalculatorUnitTest, Matrix8x8_SimpleCalculation) {
    std::map<std::string, std::vector<double>> alignedData;
    auto data1 = createLinearData(10.0, 1.0);
    auto data2 = createLinearData(20.0, 2.0);

    alignedData["indicator1"] = data1;
    alignedData["indicator2"] = data2;

    CovarianceMatrix covMat = calculator.calculateCovarianceMatrix(alignedData);

    // Verify it's 2x2
    Eigen::MatrixXd mat = covMat.getMatrix();
    EXPECT_EQ(mat.rows(), 2);
    EXPECT_EQ(mat.cols(), 2);

    // Verify it's symmetric
    EXPECT_DOUBLE_EQ(mat(0, 1), mat(1, 0));
}

TEST_F(CovarianceCalculatorUnitTest, Matrix8x8_Symmetry) {
    std::map<std::string, std::vector<double>> alignedData;
    alignedData["a"] = createLinearData(100.0, 5.0);
    alignedData["b"] = createLinearData(50.0, 3.0);
    alignedData["c"] = createLinearData(75.0, 2.0);

    CovarianceMatrix covMat = calculator.calculateCovarianceMatrix(alignedData);
    Eigen::MatrixXd mat = covMat.getMatrix();

    // Verify symmetry for all pairs
    for (int i = 0; i < mat.rows(); i++) {
        for (int j = i; j < mat.cols(); j++) {
            EXPECT_NEAR(mat(i, j), mat(j, i), 1e-10)
                << "Symmetry violated at (" << i << "," << j << ")";
        }
    }
}

TEST_F(CovarianceCalculatorUnitTest, Matrix8x8_DiagonalIsVariance) {
    std::map<std::string, std::vector<double>> alignedData;
    auto data = createLinearData(100.0, 10.0);
    alignedData["indicator"] = data;

    CovarianceMatrix covMat = calculator.calculateCovarianceMatrix(alignedData);
    Eigen::MatrixXd mat = covMat.getMatrix();

    // Diagonal element should be variance of the data
    double mean = 0.0;
    for (const auto& val : data) {
        mean += val;
    }
    mean /= data.size();

    double variance = 0.0;
    for (const auto& val : data) {
        variance += (val - mean) * (val - mean);
    }
    variance /= (data.size() - 1);

    EXPECT_NEAR(mat(0, 0), variance, 1e-10);
}

TEST_F(CovarianceCalculatorUnitTest, Matrix8x8_AllNonNegativeDiagonal) {
    std::map<std::string, std::vector<double>> alignedData;
    alignedData["a"] = createLinearData(100.0, 5.0);
    alignedData["b"] = createLinearData(50.0, 3.0);
    alignedData["c"] = createLinearData(75.0, 2.0);

    CovarianceMatrix covMat = calculator.calculateCovarianceMatrix(alignedData);
    Eigen::MatrixXd mat = covMat.getMatrix();

    // All diagonal values (variances) should be non-negative
    for (int i = 0; i < mat.rows(); i++) {
        EXPECT_GE(mat(i, i), 0.0) << "Negative variance at position " << i;
    }
}

TEST_F(CovarianceCalculatorUnitTest, Matrix8x8_FrobeniusNorm) {
    std::map<std::string, std::vector<double>> alignedData;
    alignedData["a"] = createLinearData(100.0, 5.0);
    alignedData["b"] = createLinearData(50.0, 3.0);

    CovarianceMatrix covMat = calculator.calculateCovarianceMatrix(alignedData);

    // Frobenius norm should be positive and finite
    double norm = covMat.getFrobeniusNorm();
    EXPECT_GT(norm, 0.0);
    EXPECT_FALSE(std::isnan(norm));
    EXPECT_FALSE(std::isinf(norm));
}

TEST_F(CovarianceCalculatorUnitTest, Matrix8x8_EmptyDataThrows) {
    std::map<std::string, std::vector<double>> emptyData;

    EXPECT_THROW({
        calculator.calculateCovarianceMatrix(emptyData);
    }, std::invalid_argument);
}

TEST_F(CovarianceCalculatorUnitTest, Matrix8x8_MismatchedLengthsThrows) {
    std::map<std::string, std::vector<double>> alignedData;
    alignedData["a"] = createLinearData(100.0, 5.0);
    alignedData["b"] = std::vector<double>(10, 50.0);  // Different length

    EXPECT_THROW({
        calculator.calculateCovarianceMatrix(alignedData);
    }, std::invalid_argument);
}

TEST_F(CovarianceCalculatorUnitTest, Matrix8x8_InsufficientDataThrows) {
    std::map<std::string, std::vector<double>> alignedData;
    alignedData["a"] = std::vector<double>(1, 100.0);  // Only 1 observation

    EXPECT_THROW({
        calculator.calculateCovarianceMatrix(alignedData);
    }, std::invalid_argument);
}

TEST_F(CovarianceCalculatorUnitTest, Matrix8x8_FullEightIndicators) {
    std::map<std::string, std::vector<double>> alignedData;

    // All 8 indicators with different trends
    alignedData["inflation"] = createLinearData(315.0, 0.1);
    alignedData["fed_funds"] = createLinearData(5.3, -0.05);
    alignedData["unemployment"] = createLinearData(3.8, 0.02);
    alignedData["consumer_sentiment"] = createLinearData(102.0, 1.0);
    alignedData["gdp"] = createLinearData(25000.0, 100.0);
    alignedData["treasury_10y"] = createLinearData(4.3, -0.05);
    alignedData["inverted_yield"] = createLinearData(0.1, 0.01);
    alignedData["vix"] = createLinearData(18.0, 0.5);

    CovarianceMatrix covMat = calculator.calculateCovarianceMatrix(alignedData);
    Eigen::MatrixXd mat = covMat.getMatrix();

    // Verify 8x8
    EXPECT_EQ(mat.rows(), 8);
    EXPECT_EQ(mat.cols(), 8);

    // Verify all diagonal elements are non-negative
    for (int i = 0; i < 8; i++) {
        EXPECT_GE(mat(i, i), 0.0);
    }

    // Verify Frobenius norm is positive
    EXPECT_GT(covMat.getFrobeniusNorm(), 0.0);
}

// ===== Data Validation Tests =====

TEST_F(CovarianceCalculatorUnitTest, Validation_AllIndicatorsPresentOptional) {
    // Test that we can handle missing indicators (not all 8 required)
    std::map<std::string, std::vector<double>> alignedData;
    alignedData["inflation"] = createLinearData(315.0, 0.1);
    alignedData["fed_funds"] = createLinearData(5.3, -0.05);
    alignedData["unemployment"] = createLinearData(3.8, 0.02);

    // Should not throw - fewer than 8 is okay
    EXPECT_NO_THROW({
        calculator.calculateCovarianceMatrix(alignedData);
    });
}

TEST_F(CovarianceCalculatorUnitTest, Validation_DataWithNaNThrows) {
    std::map<std::string, std::vector<double>> alignedData;
    auto data = createLinearData(100.0, 5.0);
    data[0] = std::nan("");  // Insert NaN
    alignedData["a"] = data;

    EXPECT_THROW({
        calculator.calculateCovarianceMatrix(alignedData);
    }, std::runtime_error);  // Negative variance from NaN
}

// Run tests
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
