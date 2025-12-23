//
//  debug_fred.cpp
//  Debug program to see raw FRED API responses
//
//  Compile with:
//  g++ -std=c++20 -I./src -I/opt/homebrew/opt/nlohmann-json/include \
//      src/DataProviders/FREDDataClient.cpp debug_fred.cpp -lcurl -o debug_fred
//

#include <iostream>
#include <cstdlib>
#include "DataProviders/FREDDataClient.hpp"

int main() {
    const char* apiKeyEnv = std::getenv("FRED_API_KEY");
    if (!apiKeyEnv || std::string(apiKeyEnv).empty()) {
        std::cerr << "Error: FRED_API_KEY environment variable not set\n";
        return 1;
    }

    std::string apiKey(apiKeyEnv);

    try {
        std::cout << "Testing FRED API with key: " << apiKey.substr(0, 8) << "...\n\n";

        FREDDataClient client(apiKey);

        // Test a simple series fetch and print raw response
        std::cout << "=== Fetching CPI series (CPIAUCSL) ===\n";
        std::string rawResponse = client.fetchSeries("CPIAUCSL", 3, "desc");

        std::cout << "\nRaw JSON Response:\n";
        std::cout << rawResponse << "\n\n";

        // Try to parse it
        std::cout << "=== Attempting to parse response ===\n";
        auto observations = client.fetchLatestValue("CPIAUCSL", 3);

        std::cout << "Successfully parsed " << observations.size() << " observations:\n";
        for (const auto& obs : observations) {
            std::cout << "  " << obs.date << ": " << obs.value << "\n";
        }

    } catch (const std::exception& e) {
        std::cerr << "\nError: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
