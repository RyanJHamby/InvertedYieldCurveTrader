//
//  UnemploymentProcessor.cpp
//  InvertedYieldCurveTrader
//
//  Created by Ryan Hamby on 12/22/25.
//

#include "UnemploymentProcessor.hpp"
#include "../DataProviders/FREDDataClient.hpp"
#include <stdexcept>
#include <iostream>

std::vector<double> UnemploymentProcessor::process(const std::string& fredApiKey, int numValues) {
    try {
        FREDDataClient client(fredApiKey);
        auto observations = client.fetchUnemployment(numValues);

        std::vector<double> values;
        values.reserve(observations.size());

        for (const auto& obs : observations) {
            values.push_back(obs.value);
        }

        return values;

    } catch (const std::exception& e) {
        std::cerr << "Error in UnemploymentProcessor: " << e.what() << std::endl;
        throw;
    }
}

double UnemploymentProcessor::getLatestValue(const std::string& fredApiKey) {
    auto values = process(fredApiKey, 1);

    if (values.empty()) {
        throw std::runtime_error("No Unemployment data available");
    }

    return values[0];
}
