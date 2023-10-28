//
//  CovarianceCalculator.cpp
//  InvertedYieldCurveTrader
//
//  Created by Ryan Hamby on 10/28/23.
//

#include "CovarianceCalculator.hpp"

double CovarianceCalculator::calculateCovarianceWithInvertedYield(std::vector<double> kpiValues,
                                                                  std::vector<double> invertedYieldValues,
                                                                  std::tuple<double, double> kpiMeanAndStdDev,
                                                                  std::tuple<double, double> invertedYieldMeanAndStdDev) {
    if (kpiValues.size() != invertedYieldValues.size()) {
        throw std::invalid_argument("Input vectors must have the same size.");
    }

    double kpiMean = std::get<0>(kpiMeanAndStdDev);
    double invertedYieldMean = std::get<0>(invertedYieldMeanAndStdDev);

    double covariance = 0.0;
    for (size_t i = 0; i < kpiValues.size(); i++) {
        double kpiDeviation = kpiValues[i] - kpiMean;
        double invertedYieldDeviation = invertedYieldValues[i] - invertedYieldMean;
        covariance += kpiDeviation * invertedYieldDeviation;
    }

    covariance /= (kpiValues.size() - 1); // Unbiased estimator

    return covariance;
};
