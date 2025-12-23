//
//  FREDDataClient.cpp
//  InvertedYieldCurveTrader
//
//  FRED API Client Implementation
//
//  Created by Ryan Hamby on 12/22/25.
//

#include "FREDDataClient.hpp"
#include <curl/curl.h>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Base URL for FRED API
const std::string FREDDataClient::BASE_URL = "https://api.stlouisfed.org/fred/series/observations";

// FRED Series IDs mapping
const std::map<std::string, std::string> FREDDataClient::SERIES_IDS = {
    {"inflation", "CPIAUCSL"},              // Consumer Price Index
    {"treasury_10y", "DGS10"},              // 10-Year Treasury Yield
    {"treasury_2y", "DGS2"},                // 2-Year Treasury Yield
    {"fed_funds_rate", "FEDFUNDS"},         // Federal Funds Rate
    {"unemployment", "UNRATE"},             // Unemployment Rate
    {"gdp", "A191RL1Q225SBEA"},             // Real GDP Growth Rate
    {"consumer_sentiment", "UMCSENT"}       // University of Michigan Consumer Sentiment
    // Note: ISM Manufacturing PMI not available on FRED (proprietary data)
    // VIX handled separately via Yahoo Finance (not a FRED series)
};

FREDDataClient::FREDDataClient(const std::string& apiKey) : apiKey_(apiKey) {
    if (apiKey_.empty()) {
        throw std::invalid_argument("FRED API key cannot be empty");
    }
}

size_t FREDDataClient::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

std::string FREDDataClient::fetchSeries(
    const std::string& seriesId,
    int limit,
    const std::string& sortOrder,
    const std::string& observationStart,
    const std::string& observationEnd
) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        throw std::runtime_error("Failed to initialize cURL");
    }

    // Build URL with query parameters
    std::ostringstream urlStream;
    urlStream << BASE_URL
              << "?series_id=" << seriesId
              << "&api_key=" << apiKey_
              << "&file_type=json"
              << "&limit=" << limit
              << "&sort_order=" << sortOrder;

    if (!observationStart.empty()) {
        urlStream << "&observation_start=" << observationStart;
    }
    if (!observationEnd.empty()) {
        urlStream << "&observation_end=" << observationEnd;
    }

    std::string url = urlStream.str();
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    std::string response;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);  // 10 second timeout

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        std::string error = "FRED API request failed for series " + seriesId + ": " +
                          std::string(curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        throw std::runtime_error(error);
    }

    curl_easy_cleanup(curl);

    // Validate response contains observations
    try {
        json jsonData = json::parse(response);
        if (!jsonData.contains("observations")) {
            // Print the actual response for debugging
            std::cerr << "FRED API Response (first 500 chars): " << response.substr(0, 500) << "\n";
            throw std::runtime_error("Invalid FRED API response: missing 'observations' key. Check response above.");
        }
    } catch (const json::exception& e) {
        std::cerr << "FRED API Response (first 500 chars): " << response.substr(0, 500) << "\n";
        throw std::runtime_error("Failed to parse FRED API response: " + std::string(e.what()));
    }

    return response;
}

std::vector<FREDObservation> FREDDataClient::fetchLatestValue(
    const std::string& seriesId,
    int numValues
) {
    std::string jsonResponse = fetchSeries(seriesId, numValues, "desc");

    std::vector<FREDObservation> observations;

    try {
        json jsonData = json::parse(jsonResponse);

        for (const auto& obs : jsonData["observations"]) {
            // Filter out missing values (marked as '.')
            std::string valueStr = obs["value"].get<std::string>();
            if (valueStr != ".") {
                FREDObservation observation;
                observation.date = obs["date"].get<std::string>();
                observation.value = std::stod(valueStr);
                observations.push_back(observation);
            }
        }
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to parse observations for series " + seriesId +
                               ": " + std::string(e.what()));
    }

    return observations;
}

std::vector<FREDObservation> FREDDataClient::fetchInflation(int numValues) {
    return fetchLatestValue(SERIES_IDS.at("inflation"), numValues);
}

std::vector<FREDObservation> FREDDataClient::fetchTreasury10Y(int numValues) {
    return fetchLatestValue(SERIES_IDS.at("treasury_10y"), numValues);
}

std::vector<FREDObservation> FREDDataClient::fetchTreasury2Y(int numValues) {
    return fetchLatestValue(SERIES_IDS.at("treasury_2y"), numValues);
}

std::vector<FREDObservation> FREDDataClient::fetchFedFundsRate(int numValues) {
    return fetchLatestValue(SERIES_IDS.at("fed_funds_rate"), numValues);
}

std::vector<FREDObservation> FREDDataClient::fetchUnemployment(int numValues) {
    return fetchLatestValue(SERIES_IDS.at("unemployment"), numValues);
}

std::vector<FREDObservation> FREDDataClient::fetchGDP(int numValues) {
    return fetchLatestValue(SERIES_IDS.at("gdp"), numValues);
}

std::vector<FREDObservation> FREDDataClient::fetchConsumerSentiment(int numValues) {
    return fetchLatestValue(SERIES_IDS.at("consumer_sentiment"), numValues);
}

std::vector<FREDObservation> FREDDataClient::fetchISMManufacturing(int numValues) {
    return fetchLatestValue(SERIES_IDS.at("ism_manufacturing"), numValues);
}

std::vector<FREDObservation> FREDDataClient::calculateInvertedYieldCurve(int numDays) {
    std::vector<FREDObservation> treasury10Y = fetchTreasury10Y(numDays);
    std::vector<FREDObservation> treasury2Y = fetchTreasury2Y(numDays);

    // Create date-indexed map for easy lookup
    std::map<std::string, double> yields10Y;
    std::map<std::string, double> yields2Y;

    for (const auto& obs : treasury10Y) {
        yields10Y[obs.date] = obs.value;
    }

    for (const auto& obs : treasury2Y) {
        yields2Y[obs.date] = obs.value;
    }

    // Calculate spreads for dates present in both series
    std::vector<FREDObservation> spreads;

    for (auto it = yields10Y.rbegin(); it != yields10Y.rend() && spreads.size() < static_cast<size_t>(numDays); ++it) {
        const std::string& date = it->first;

        if (yields2Y.find(date) != yields2Y.end()) {
            FREDObservation spread;
            spread.date = date;
            spread.value = yields10Y[date] - yields2Y[date];  // Negative = inverted
            spreads.push_back(spread);
        }
    }

    return spreads;
}
