//
//  StatsCalculator.hpp
//  InvertedYieldCurveTrader
//
//  Created by Ryan Hamby on 9/30/23.
//

#ifndef StatsCalculator_hpp
#define StatsCalculator_hpp

#include <vector>
#include <tuple>

class StatsCalculator {
public:
    void setData(const std::string& jsonData);
    std::tuple<double, double> calculateMeanAndStdDev();
    std::tuple<double, double> calculateBuyAndSellConfidence();

private:
    std::vector<double> data;
};

#endif /* StatsCalculator_hpp */
