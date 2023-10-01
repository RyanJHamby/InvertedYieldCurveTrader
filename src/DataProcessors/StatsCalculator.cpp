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

std::tuple<double, double> StatsCalculator::calculateMeanAndStdDev() {
    double mean = 0.0;
    double sum = 0.0;
    double variance = 0.0;

    if (!data.empty()) {
        // Calculate the mean
        for (double value : data) {
            sum += value;
        }
        mean = sum / data.size();

        // Calculate the variance
        for (double value : data) {
            double diff = value - mean;
            variance += diff * diff;
        }
        variance /= data.size();

        // Calculate the standard deviation
        double stdDev = std::sqrt(variance);

        return std::make_tuple(mean, stdDev);
    } else {
        return std::make_tuple(0.0, 0.0);
    }
}

std::tuple<double, double> StatsCalculator::calculateBuyAndSellConfidence() {
    auto [mean, stdDev] = calculateMeanAndStdDev();
    
    double buyConfidence = 0.0;
    double sellConfidence = 0.0;

    // Rules for setting buy and sell confidence based on stdDev and mean
    if (stdDev <= 1.0) {
        buyConfidence = 0.3; // Low buy confidence for low stdDev
        sellConfidence = 0.3; // Low sell confidence for low stdDev
    } else if (stdDev > 1.0 && stdDev <= 2.0) {
        buyConfidence = 0.5; // Average buy confidence for normal stdDev
        sellConfidence = 0.5; // Average sell confidence for normal stdDev
    } else {
        buyConfidence = 0.7; // High buy confidence for high stdDev
        sellConfidence = 0.2; // Low sell confidence for high stdDev
    }

    if (stdDev <= 1.0 && mean > 0.0) {
        sellConfidence = 0.7; // High sell confidence for low stdDev and high mean
    }

    return std::make_tuple(buyConfidence, sellConfidence);
}
