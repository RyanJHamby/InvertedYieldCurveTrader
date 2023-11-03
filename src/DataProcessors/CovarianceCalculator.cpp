//
//  CovarianceCalculator.cpp
//  InvertedYieldCurveTrader
//
//  Created by Ryan Hamby on 10/28/23.
//

#include "CovarianceCalculator.hpp"
#include <iostream>

double CovarianceCalculator::calculateCovarianceWithInvertedYield(std::vector<double> kpiValues,
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

    covariance /= (kpiValues.size() - 1); // Unbiased estimator

    return covariance;
};
