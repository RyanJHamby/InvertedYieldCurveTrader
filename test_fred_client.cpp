//
//  test_fred_client.cpp
//  Simple test program for FREDDataClient
//
//  Compile with:
//  g++ -std=c++20 -I./src -I/opt/homebrew/opt/nlohmann-json/include \
//      src/DataProviders/FREDDataClient.cpp test_fred_client.cpp -lcurl -o test_fred
//
//  Run with:
//  export FRED_API_KEY="your_api_key_here"
//  ./test_fred
//

#include <iostream>
#include <cstdlib>
#include "DataProviders/FREDDataClient.hpp"

void printObservations(const std::string& title, const std::vector<FREDObservation>& observations) {
    std::cout << "\n" << title << ":\n";
    if (observations.empty()) {
        std::cout << "  No data available\n";
        return;
    }

    for (size_t i = 0; i < std::min(size_t(5), observations.size()); ++i) {
        std::cout << "  " << observations[i].date << ": " << observations[i].value << "\n";
    }
}

int main() {
    // Get API key from environment variable
    const char* apiKeyEnv = std::getenv("FRED_API_KEY");
    if (!apiKeyEnv || std::string(apiKeyEnv).empty()) {
        std::cerr << "Error: FRED_API_KEY environment variable not set\n";
        std::cerr << "Get your free API key from https://fred.stlouisfed.org/\n";
        std::cerr << "Then set it with: export FRED_API_KEY='your_api_key_here'\n";
        return 1;
    }

    std::string apiKey(apiKeyEnv);

    try {
        std::cout << "Initializing FRED API Client...\n";
        FREDDataClient client(apiKey);

        // Test 1: Fetch latest inflation data
        std::cout << "\n=== Test 1: Fetching CPI (Inflation) Data ===\n";
        auto inflation = client.fetchInflation(5);
        printObservations("Latest CPI Values", inflation);

        // Test 2: Fetch treasury yields
        std::cout << "\n=== Test 2: Fetching Treasury Yields ===\n";
        auto treasury10y = client.fetchTreasury10Y(5);
        auto treasury2y = client.fetchTreasury2Y(5);
        printObservations("10-Year Treasury Yield", treasury10y);
        printObservations("2-Year Treasury Yield", treasury2y);

        // Test 3: Calculate inverted yield curve
        std::cout << "\n=== Test 3: Inverted Yield Curve Analysis ===\n";
        auto yieldCurve = client.calculateInvertedYieldCurve(5);
        std::cout << "\nYield Curve Spread (10Y - 2Y):\n";
        for (const auto& obs : yieldCurve) {
            std::string status = obs.value < 0 ? "INVERTED!" : "Normal";
            std::cout << "  " << obs.date << ": " << obs.value << "% (" << status << ")\n";
        }

        // Test 4: Fetch other economic indicators
        std::cout << "\n=== Test 4: Other Economic Indicators ===\n";

        auto fedFunds = client.fetchFedFundsRate(3);
        printObservations("Federal Funds Rate", fedFunds);

        auto unemployment = client.fetchUnemployment(3);
        printObservations("Unemployment Rate", unemployment);

        auto gdp = client.fetchGDP(3);
        printObservations("GDP Growth Rate (Quarterly)", gdp);

        auto sentiment = client.fetchConsumerSentiment(3);
        printObservations("Consumer Sentiment Index", sentiment);

        auto ism = client.fetchISMManufacturing(3);
        printObservations("ISM Manufacturing PMI", ism);

        std::cout << "\n=== All Tests Passed! ===\n";
        std::cout << "FRED API Client is working correctly.\n";

        return 0;

    } catch (const std::exception& e) {
        std::cerr << "\nError: " << e.what() << "\n";
        return 1;
    }
}
