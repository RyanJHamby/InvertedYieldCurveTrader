//
//  VIXDataProcessor.cpp
//  InvertedYieldCurveTrader
//
//  Fetches VIX from Alpha Vantage
//
//  Created by Ryan Hamby on 12/22/25.
//

#include "VIXDataProcessor.hpp"
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <algorithm>

using json = nlohmann::json;

size_t VIXDataProcessor::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

std::string VIXDataProcessor::fetchFromAlphaVantage(const std::string& apiKey) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        throw std::runtime_error("Failed to initialize cURL for VIX fetch");
    }

    // Alpha Vantage API endpoint for VIX
    // Using TIME_SERIES_DAILY with symbol VIX
    std::ostringstream urlStream;
    urlStream << "https://www.alphavantage.co/query"
              << "?function=TIME_SERIES_DAILY"
              << "&symbol=VIX"
              << "&outputsize=compact"  // Last 100 days
              << "&apikey=" << apiKey;

    std::string url = urlStream.str();
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    std::string response;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        std::string error = "VIX fetch failed: " + std::string(curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        throw std::runtime_error(error);
    }

    curl_easy_cleanup(curl);

    if (response.empty()) {
        throw std::runtime_error("VIX: Alpha Vantage returned empty response");
    }

    return response;
}

std::vector<double> VIXDataProcessor::parseAlphaVantageResponse(const std::string& jsonData, int numDays) {
    std::vector<double> values;

    try {
        json j = json::parse(jsonData);

        // Check for API error
        if (j.contains("Error Message")) {
            throw std::runtime_error("Alpha Vantage API error: " + j["Error Message"].get<std::string>());
        }

        if (j.contains("Note")) {
            throw std::runtime_error("Alpha Vantage rate limit: " + j["Note"].get<std::string>());
        }

        // Get time series data
        if (!j.contains("Time Series (Daily)")) {
            throw std::runtime_error("Invalid Alpha Vantage response: missing 'Time Series (Daily)' key");
        }

        auto timeSeries = j["Time Series (Daily)"];

        // Create vector of (date, close_price) pairs
        std::vector<std::pair<std::string, double>> dataPoints;
        for (auto& [date, dayData] : timeSeries.items()) {
            std::string closeStr = dayData["4. close"].get<std::string>();
            double closePrice = std::stod(closeStr);
            dataPoints.push_back({date, closePrice});
        }

        // Sort by date descending (most recent first)
        std::sort(dataPoints.begin(), dataPoints.end(),
                  [](const auto& a, const auto& b) { return a.first > b.first; });

        // Extract values (up to numDays)
        for (size_t i = 0; i < std::min(static_cast<size_t>(numDays), dataPoints.size()); ++i) {
            values.push_back(dataPoints[i].second);
        }

    } catch (const json::exception& e) {
        throw std::runtime_error("Failed to parse VIX data: " + std::string(e.what()));
    }

    return values;
}

std::vector<double> VIXDataProcessor::process(const std::string& alphaVantageApiKey, int numDays) {
    try {
        std::string jsonData = fetchFromAlphaVantage(alphaVantageApiKey);
        return parseAlphaVantageResponse(jsonData, numDays);
    } catch (const std::exception& e) {
        std::cerr << "Error in VIXDataProcessor: " << e.what() << std::endl;
        throw;
    }
}

double VIXDataProcessor::getLatestValue(const std::string& alphaVantageApiKey) {
    auto values = process(alphaVantageApiKey, 1);

    if (values.empty()) {
        throw std::runtime_error("No VIX data available");
    }

    return values[0];
}
