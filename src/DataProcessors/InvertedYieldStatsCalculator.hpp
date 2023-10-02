//
//  InvertedYieldStatsCalculator.hpp
//  InvertedYieldCurveTrader
//
//  Created by Ryan Hamby on 10/1/23.
//

#ifndef InvertedYieldStatsCalculator_hpp
#define InvertedYieldStatsCalculator_hpp

#include <stdio.h>
#include <tuple>
#include <vector>
#include "StatsCalculator.hpp"

class InvertedYieldStatsCalculator : public StatsCalculator {
public:
    void setData(const std::string& jsonData10year, const std::string& jsonData2year);

private:
    std::vector<std::vector<double>> data2D;
};

#endif /* InvertedYieldStatsCalculator_hpp */
