//
//  FedFundsProcessor.hpp
//  InvertedYieldCurveTrader
//
//  Processes Federal Funds Rate data from FRED API
//
//  Created by Ryan Hamby on 12/22/25.
//

#ifndef FedFundsProcessor_hpp
#define FedFundsProcessor_hpp

#include <vector>
#include <string>

class FedFundsProcessor {
public:
    // Fetch and process Federal Funds Rate data
    // Returns vector of recent values (most recent first)
    std::vector<double> process(const std::string& fredApiKey, int numValues = 12);

    // Get the latest single value
    double getLatestValue(const std::string& fredApiKey);
};

#endif /* FedFundsProcessor_hpp */
