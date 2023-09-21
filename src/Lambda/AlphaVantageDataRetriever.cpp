//
//  AlphaVantageDataRetriever.cpp
//  InvertedYieldCurveTrader
//
//  Created by Ryan Hamby on 9/19/23.
//

#include "AlphaVantageDataRetriever.hpp"
#include <curl/curl.h>

AlphaVantageDataRetriever::AlphaVantageDataRetriever(const std::string& apiKey, int maxResults)
    : apiKey_(apiKey), maxResults_(maxResults) {}

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

std::string AlphaVantageDataRetriever::retrieveStockData(const std::string& symbol) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        return "Failed to initialize cURL.";
    }

    std::string apiUrl = "https://www.alphavantage.co/query?function=TIME_SERIES_INTRADAY&symbol=" +
                        symbol + "&interval=1min&apikey=" + apiKey_;

    curl_easy_setopt(curl, CURLOPT_URL, apiUrl.c_str());

    std::string response;

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        return "cURL request failed: " + std::string(curl_easy_strerror(res));
    }

    curl_easy_cleanup(curl);

    return response;
}
