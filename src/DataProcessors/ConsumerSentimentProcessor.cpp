//
//  ConsumerSentimentProcessor.cpp
//  InvertedYieldCurveTrader
//
//  Created by Ryan Hamby on 12/22/25.
//

#include "ConsumerSentimentProcessor.hpp"
#include "../DataProviders/FREDDataClient.hpp"
#include <stdexcept>
#include <iostream>

std::vector<double> ConsumerSentimentProcessor::process(const std::string& fredApiKey, int numValues) {
    try {
        FREDDataClient client(fredApiKey);
        auto observations = client.fetchConsumerSentiment(numValues);

        std::vector<double> values;
        values.reserve(observations.size());

        for (const auto& obs : observations) {
            values.push_back(obs.value);
        }

        return values;

    } catch (const std::exception& e) {
        std::cerr << "Error in ConsumerSentimentProcessor: " << e.what() << std::endl;
        throw;
    }
}

double ConsumerSentimentProcessor::getLatestValue(const std::string& fredApiKey) {
    auto values = process(fredApiKey, 1);

    if (values.empty()) {
        throw std::runtime_error("No Consumer Sentiment data available");
    }

    return values[0];
}
