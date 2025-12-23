//
//  UnemploymentProcessor.hpp
//  InvertedYieldCurveTrader
//
//  Processes Unemployment Rate data from FRED API
//
//  Created by Ryan Hamby on 12/22/25.
//

#ifndef UnemploymentProcessor_hpp
#define UnemploymentProcessor_hpp

#include <vector>
#include <string>

class UnemploymentProcessor {
public:
    // Fetch and process Unemployment Rate data
    // Returns vector of recent values (most recent first)
    std::vector<double> process(const std::string& fredApiKey, int numValues = 12);

    // Get the latest single value
    double getLatestValue(const std::string& fredApiKey);
};

#endif /* UnemploymentProcessor_hpp */
