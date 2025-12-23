//
//  FedFundsProcessor.cpp
//  InvertedYieldCurveTrader
//
//  Created by Ryan Hamby on 12/22/25.
//

#include "FedFundsProcessor.hpp"
#include "../DataProviders/FREDDataClient.hpp"
#include <stdexcept>
#include <iostream>

std::vector<double> FedFundsProcessor::process(const std::string& fredApiKey, int numValues) {
    try {
        FREDDataClient client(fredApiKey);
        auto observations = client.fetchFedFundsRate(numValues);

        std::vector<double> values;
        values.reserve(observations.size());

        for (const auto& obs : observations) {
            values.push_back(obs.value);
        }

        return values;

    } catch (const std::exception& e) {
        std::cerr << "Error in FedFundsProcessor: " << e.what() << std::endl;
        throw;
    }
}

double FedFundsProcessor::getLatestValue(const std::string& fredApiKey) {
    auto values = process(fredApiKey, 1);

    if (values.empty()) {
        throw std::runtime_error("No Federal Funds Rate data available");
    }

    return values[0];
}
