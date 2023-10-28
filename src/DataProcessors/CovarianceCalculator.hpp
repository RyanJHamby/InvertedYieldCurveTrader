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

class CovarianceCalculator {
public:
    double calculateCovarianceWithInvertedYield(std::vector<double> kpiValues,
                                                std::vector<double> invertedYieldValues,
                                                std::tuple<double, double> kpiMeanAndStdDev,
                                                std::tuple<double, double> invertedYieldMeanAndStdDev);
};

#endif /* CovarianceCalculator_hpp */
