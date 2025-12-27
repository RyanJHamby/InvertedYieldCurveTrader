//
//  GDPDataProcessor.hpp
//  InvertedYieldCurveTrader
//
//  Created by Ryan Hamby on 10/30/23.
//

#ifndef GDPDataProcessor_hpp
#define GDPDataProcessor_hpp

#include <stdio.h>
#include <vector>
#include <string>

class GDPDataProcessor {
public:
    std::vector<double> process(const std::string& fredApiKey, int numValues = 8);
};

#endif /* GDPDataProcessor_hpp */
