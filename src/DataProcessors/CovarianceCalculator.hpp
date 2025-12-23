//
//  CovarianceCalculator.hpp
//  InvertedYieldCurveTrader
//
//  Created by Ryan Hamby on 10/28/23.
//

#ifndef CovarianceCalculator_hpp
#define CovarianceCalculator_hpp

#include <stdio.h>
#include <vector>
#include <tuple>
#include <map>
#include <string>
#include <Eigen/Dense>
#include <iostream>

/**
 * Holds an 8x8 covariance matrix with metadata
 * Provides methods to access covariance values and compute statistics
 */
class CovarianceMatrix {
private:
    Eigen::MatrixXd matrix_;
    std::vector<std::string> indicatorNames_;

public:
    /**
     * Construct a CovarianceMatrix
     *
     * @param matrix 8x8 Eigen matrix of covariance values
     * @param names Vector of 8 indicator names (must match matrix dimensions)
     */
    CovarianceMatrix(const Eigen::MatrixXd& matrix,
                     const std::vector<std::string>& names);

    /**
     * Get covariance between two indicators by name
     *
     * @param indicator1 Name of first indicator
     * @param indicator2 Name of second indicator
     * @return Covariance value between the two indicators
     * @throws std::out_of_range if indicator names not found
     */
    double getCovariance(const std::string& indicator1,
                        const std::string& indicator2) const;

    /**
     * Get the underlying Eigen matrix (8x8)
     * @return Reference to the covariance matrix
     */
    Eigen::MatrixXd getMatrix() const;

    /**
     * Get the indicator names in the order they appear in the matrix
     * @return Vector of 8 indicator names
     */
    std::vector<std::string> getIndicatorNames() const;

    /**
     * Calculate Frobenius norm of the covariance matrix
     * Represents overall economic regime volatility
     *
     * Formula: ||Cov||_F = sqrt(sum of all squared covariance values)
     *
     * @return Frobenius norm as a double
     */
    double getFrobeniusNorm() const;

    /**
     * Print the covariance matrix in human-readable format
     * Useful for debugging and visualization
     */
    void print() const;
};

/**
 * Calculates covariance between economic indicators
 * Supports both pairwise (2x2) and multivariate (8x8) covariance
 */
class CovarianceCalculator {
public:
    /**
     * Calculate pairwise covariance between two vectors (backward compatible)
     * Uses unbiased estimator: Cov(X,Y) = Σ[(xi - μx)(yi - μy)] / (n - 1)
     *
     * @param kpiValues Vector of first indicator values
     * @param invertedYieldValues Vector of second indicator values
     * @param kpiMean Pre-calculated mean of first indicator
     * @param invertedYieldMean Pre-calculated mean of second indicator
     * @return Single covariance value
     */
    double calculateCovarianceWithInvertedYield(std::vector<double> kpiValues,
                                                std::vector<double> invertedYieldValues,
                                                double kpiMean,
                                                double invertedYieldMean);

    /**
     * Calculate full 8x8 covariance matrix from aligned indicator data
     *
     * Requirement: All indicators in alignedData must have the same number of observations
     * (typically 12 monthly values after alignment via DataAligner)
     *
     * @param alignedData Map of indicator names to their aligned time series (must have 12 values each)
     * @return CovarianceMatrix object containing 8x8 covariance matrix
     * @throws std::invalid_argument if data is missing or misaligned
     */
    CovarianceMatrix calculateCovarianceMatrix(
        const std::map<std::string, std::vector<double>>& alignedData
    );

private:
    /**
     * Internal helper to compute means for each indicator
     */
    static std::map<std::string, double> computeMeans(
        const std::map<std::string, std::vector<double>>& data
    );

    /**
     * Internal helper to validate input data for matrix calculation
     */
    static void validateData(const std::map<std::string, std::vector<double>>& data);
};

#endif /* CovarianceCalculator_hpp */
