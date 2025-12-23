//
//  VIXDataProcessor.hpp
//  InvertedYieldCurveTrader
//
//  Processes VIX (Volatility Index) data from Alpha Vantage
//  VIX is not available on FRED - it's a CBOE product
//
//  Created by Ryan Hamby on 12/22/25.
//

#ifndef VIXDataProcessor_hpp
#define VIXDataProcessor_hpp

#include <vector>
#include <string>

class VIXDataProcessor {
public:
    // Fetch and process VIX data from Alpha Vantage
    // Returns vector of recent closing prices (most recent first)
    std::vector<double> process(const std::string& alphaVantageApiKey, int numDays = 30);

    // Get the latest VIX value
    double getLatestValue(const std::string& alphaVantageApiKey);

private:
    // Fetch VIX data from Alpha Vantage API
    std::string fetchFromAlphaVantage(const std::string& apiKey);

    // Parse Alpha Vantage JSON response
    std::vector<double> parseAlphaVantageResponse(const std::string& jsonData, int numDays);

    // cURL callback
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output);
};

#endif /* VIXDataProcessor_hpp */
