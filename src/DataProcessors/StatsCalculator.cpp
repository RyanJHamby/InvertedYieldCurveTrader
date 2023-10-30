//
//  StatsCalculator.cpp
//  InvertedYieldCurveTrader
//
//  Created by Ryan Hamby on 9/30/23.
//

#include "StatsCalculator.hpp"
#include <nlohmann/json.hpp>
#include <cmath>
#include <iostream>
using json = nlohmann::json;

void StatsCalculator::setData(const std::string& jsonData) {
    try {
        nlohmann::json inflationData = nlohmann::json::parse(jsonData);

        std::vector<double> values;
        for (const auto& entry : inflationData["data"]) {
            // Convert the "value" field from string to double and add it to the vector
            double value = std::stod(entry["value"].get<std::string>());
            values.push_back(value);
        }

        data = values;
    } catch (const nlohmann::json::exception& e) {
        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
    }
}

std::vector<double> StatsCalculator::getData() {
    return data;
};

double StatsCalculator::calculateMean(const std::vector<double>& data) {
    if (data.empty()) {
        return 0.0;
    }

    double sum = 0.0;
    for (double value : data) {
        sum += value;
    }
    
    return sum / data.size();
}
