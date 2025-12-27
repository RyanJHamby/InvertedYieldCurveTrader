//
//  InflationDataProcessor.hpp
//  InvertedYieldCurveTrader
//
//  Created by Ryan Hamby on 9/21/23.
//

#ifndef InflationDataProcessor_hpp
#define InflationDataProcessor_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>

class InflationDataProcessor {
public:
    std::vector<double> process(const std::string& fredApiKey, int numValues = 10);
};

#endif /* InflationDataProcessor_hpp */
