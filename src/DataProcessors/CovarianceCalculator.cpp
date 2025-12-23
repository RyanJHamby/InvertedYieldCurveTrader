//
//  CovarianceCalculator.cpp
//  InvertedYieldCurveTrader
//
//  Implementation of covariance calculations for 2x2 and 8x8 matrices
//
//  Created by Ryan Hamby on 10/28/23.
//

#include "CovarianceCalculator.hpp"
#include <cmath>
#include <stdexcept>
#include <algorithm>
#include <iomanip>

// ===== CovarianceMatrix Implementation =====

CovarianceMatrix::CovarianceMatrix(const Eigen::MatrixXd& matrix,
                                   const std::vector<std::string>& names)
    : matrix_(matrix), indicatorNames_(names) {
    if (matrix.rows() != matrix.cols()) {
        throw std::invalid_argument("Covariance matrix must be square");
    }
    if (static_cast<size_t>(matrix.rows()) != names.size()) {
        throw std::invalid_argument("Matrix dimension must match number of indicator names");
    }
}

double CovarianceMatrix::getCovariance(const std::string& indicator1,
                                       const std::string& indicator2) const {
    // Find indices of the indicators
    int idx1 = -1, idx2 = -1;

    for (size_t i = 0; i < indicatorNames_.size(); i++) {
        if (indicatorNames_[i] == indicator1) idx1 = i;
        if (indicatorNames_[i] == indicator2) idx2 = i;
    }

    if (idx1 == -1) {
        throw std::out_of_range("Indicator '" + indicator1 + "' not found in covariance matrix");
    }
    if (idx2 == -1) {
        throw std::out_of_range("Indicator '" + indicator2 + "' not found in covariance matrix");
    }

    return matrix_(idx1, idx2);
}

Eigen::MatrixXd CovarianceMatrix::getMatrix() const {
    return matrix_;
}

std::vector<std::string> CovarianceMatrix::getIndicatorNames() const {
    return indicatorNames_;
}

double CovarianceMatrix::getFrobeniusNorm() const {
    return matrix_.norm();  // Eigen's built-in Frobenius norm
}

void CovarianceMatrix::print() const {
    std::cout << "\n8x8 Covariance Matrix:\n";
    std::cout << std::string(80, '=') << "\n";

    // Print header row with indicator names
    std::cout << std::setw(20) << "Indicator";
    for (const auto& name : indicatorNames_) {
        std::cout << std::setw(12) << name.substr(0, 10);
    }
    std::cout << "\n" << std::string(80, '-') << "\n";

    // Print rows
    for (size_t i = 0; i < indicatorNames_.size(); i++) {
        std::cout << std::setw(20) << indicatorNames_[i];
        for (size_t j = 0; j < indicatorNames_.size(); j++) {
            std::cout << std::setw(12) << std::fixed << std::setprecision(4) << matrix_(i, j);
        }
        std::cout << "\n";
    }

    std::cout << std::string(80, '=') << "\n";
    std::cout << "Frobenius Norm: " << std::fixed << std::setprecision(6) << getFrobeniusNorm() << "\n\n";
}

// ===== CovarianceCalculator Implementation =====

double CovarianceCalculator::calculateCovarianceWithInvertedYield(
    std::vector<double> kpiValues,
    std::vector<double> invertedYieldValues,
    double kpiMean,
    double invertedYieldMean) {

    if (kpiValues.size() != invertedYieldValues.size()) {
        throw std::invalid_argument("Input vectors must have the same size.");
    }

    double covariance = 0.0;
    for (size_t i = 0; i < kpiValues.size(); i++) {
        double kpiDeviation = kpiValues[i] - kpiMean;
        double invertedYieldDeviation = invertedYieldValues[i] - invertedYieldMean;
        covariance += kpiDeviation * invertedYieldDeviation;
    }

    covariance /= (kpiValues.size() - 1);  // Unbiased estimator

    return covariance;
}

std::map<std::string, double> CovarianceCalculator::computeMeans(
    const std::map<std::string, std::vector<double>>& data) {

    std::map<std::string, double> means;

    for (const auto& [indicator, values] : data) {
        if (values.empty()) {
            throw std::invalid_argument("Empty data vector for indicator: " + indicator);
        }

        double sum = 0.0;
        for (const auto& val : values) {
            sum += val;
        }

        means[indicator] = sum / values.size();
    }

    return means;
}

void CovarianceCalculator::validateData(
    const std::map<std::string, std::vector<double>>& data) {

    if (data.empty()) {
        throw std::invalid_argument("Data map cannot be empty");
    }

    // Check that all indicators have the same number of observations
    size_t expectedSize = 0;
    for (const auto& [indicator, values] : data) {
        if (expectedSize == 0) {
            expectedSize = values.size();
        } else if (values.size() != expectedSize) {
            throw std::invalid_argument(
                "All indicators must have the same number of observations. "
                "Expected " + std::to_string(expectedSize) + ", got " +
                std::to_string(values.size()) + " for " + indicator
            );
        }

        // Check for NaN values
        for (size_t i = 0; i < values.size(); i++) {
            if (std::isnan(values[i])) {
                throw std::runtime_error(
                    "NaN value found in indicator '" + indicator + "' at index " + std::to_string(i)
                );
            }
        }
    }

    if (expectedSize < 2) {
        throw std::invalid_argument("Need at least 2 observations per indicator for covariance calculation");
    }
}

CovarianceMatrix CovarianceCalculator::calculateCovarianceMatrix(
    const std::map<std::string, std::vector<double>>& alignedData) {

    // Validate input
    validateData(alignedData);

    // Compute means for all indicators
    auto means = computeMeans(alignedData);

    // Get number of observations
    size_t numObservations = alignedData.begin()->second.size();

    // Create ordered list of indicator names for consistent matrix indexing
    std::vector<std::string> indicatorNames;
    for (const auto& [indicator, _] : alignedData) {
        indicatorNames.push_back(indicator);
    }
    std::sort(indicatorNames.begin(), indicatorNames.end());  // Sort for consistency

    // Create 8x8 covariance matrix
    int numIndicators = indicatorNames.size();
    Eigen::MatrixXd covMatrix(numIndicators, numIndicators);

    // Calculate covariance for each pair of indicators
    for (int i = 0; i < numIndicators; i++) {
        for (int j = 0; j < numIndicators; j++) {
            double covariance = 0.0;

            const std::string& ind_i = indicatorNames[i];
            const std::string& ind_j = indicatorNames[j];
            const auto& data_i = alignedData.at(ind_i);
            const auto& data_j = alignedData.at(ind_j);
            double mean_i = means.at(ind_i);
            double mean_j = means.at(ind_j);

            // Compute covariance: Cov(X,Y) = Σ[(xi - μx)(yi - μy)] / (n - 1)
            for (size_t k = 0; k < numObservations; k++) {
                double deviation_i = data_i[k] - mean_i;
                double deviation_j = data_j[k] - mean_j;
                covariance += deviation_i * deviation_j;
            }

            // Unbiased estimator
            covariance /= (numObservations - 1);
            covMatrix(i, j) = covariance;
        }
    }

    // Validate matrix properties
    // Check symmetry: Cov(i,j) should equal Cov(j,i)
    for (int i = 0; i < numIndicators; i++) {
        for (int j = i + 1; j < numIndicators; j++) {
            double diff = std::abs(covMatrix(i, j) - covMatrix(j, i));
            if (diff > 1e-10) {
                std::cerr << "Warning: Covariance matrix not perfectly symmetric at ("
                          << i << "," << j << "): " << covMatrix(i, j) << " vs "
                          << covMatrix(j, i) << std::endl;
            }
        }
    }

    // Check diagonal (variance) values are non-negative
    for (int i = 0; i < numIndicators; i++) {
        if (covMatrix(i, i) < 0) {
            throw std::runtime_error(
                "Negative variance found for " + indicatorNames[i] +
                ": " + std::to_string(covMatrix(i, i))
            );
        }
    }

    return CovarianceMatrix(covMatrix, indicatorNames);
}
