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
        for (int i = 0; i < 1000; ++i) {
            auto entry10Year = inflationData10Year["data"][i];
            auto entry2Year = inflationData2Year["data"][i];
                        
            if (entry10Year["value"] != "." && entry2Year["value"] != ".") {
                //  Convert the "value" field from string to double and add it to the vector
                double value10Year = std::stod(entry10Year["value"].get<std::string>());
                double value2Year = std::stod(entry2Year["value"].get<std::string>());
            
                // Inverted Yield = 10 year yield - 2 year yield
                values.push_back(value10Year - value2Year);
            } else {
                // Maintain previous day for missing data, and maintain day ordering
                values.push_back(values.back());
            }
        }
        
        data = values;
    } catch (const nlohmann::json::exception& e) {
        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
    }
}
