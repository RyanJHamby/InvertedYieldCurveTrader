//
//  InvertedYieldStatsCalculator.cpp
//  InvertedYieldCurveTrader
//
//  Created by Ryan Hamby on 10/1/23.
//

#include "InvertedYieldStatsCalculator.hpp"
#include <nlohmann/json.hpp>
#include <iostream>
using json = nlohmann::json;

void InvertedYieldStatsCalculator::setData(const std::string& jsonData10Year, const std::string& jsonData2Year) {
    try {
        nlohmann::json inflationData10Year = nlohmann::json::parse(jsonData10Year);
        nlohmann::json inflationData2Year = nlohmann::json::parse(jsonData2Year);

        std::vector<double> values;
        for (int i = 0; i < inflationData10Year["data"].size(); ++i) {
            auto entry10Year = inflationData10Year["data"][i];
            auto entry2Year = inflationData2Year["data"][i];
            
            // Convert the "value" field from string to double and add it to the vector
            double value10Year = double(entry10Year["value"]);
            double value2Year = double(entry2Year["value"]);
            
            // Inverted Yield = 10 year yield - 2 year yield
            values.push_back(value10Year - value2Year);
        }
        
        data = values;
    } catch (const nlohmann::json::exception& e) {
        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
    }
}
