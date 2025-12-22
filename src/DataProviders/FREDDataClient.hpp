//
//  FREDDataClient.hpp
//  InvertedYieldCurveTrader
//
//  FRED API Client - Federal Reserve Economic Data
//  Fetches economic indicators from the St. Louis Fed FRED API
//  Documentation: https://fred.stlouisfed.org/docs/api/fred/
//
//  Created by Ryan Hamby on 12/22/25.
//

#ifndef FREDDataClient_hpp
#define FREDDataClient_hpp

#include <string>
#include <map>
#include <vector>

struct FREDObservation {
    std::string date;
    double value;
};

class FREDDataClient {
public:
    // FRED Series IDs for economic indicators
    static const std::map<std::string, std::string> SERIES_IDS;

    explicit FREDDataClient(const std::string& apiKey);

    // Core fetch method - retrieves observations for a specific FRED series
    std::string fetchSeries(
        const std::string& seriesId,
        int limit = 100,
        const std::string& sortOrder = "desc",
        const std::string& observationStart = "",
        const std::string& observationEnd = ""
    );

    // Convenience method - fetches and parses latest N values for a series
    std::vector<FREDObservation> fetchLatestValue(
        const std::string& seriesId,
        int numValues = 1
    );

    // Indicator-specific convenience methods
    std::vector<FREDObservation> fetchInflation(int numValues = 10);
    std::vector<FREDObservation> fetchTreasury10Y(int numValues = 90);
    std::vector<FREDObservation> fetchTreasury2Y(int numValues = 90);
    std::vector<FREDObservation> fetchFedFundsRate(int numValues = 12);
    std::vector<FREDObservation> fetchUnemployment(int numValues = 12);
    std::vector<FREDObservation> fetchGDP(int numValues = 8);
    std::vector<FREDObservation> fetchConsumerSentiment(int numValues = 12);
    std::vector<FREDObservation> fetchISMManufacturing(int numValues = 12);

    // Calculate inverted yield curve spread (10Y - 2Y)
    // Negative spread indicates inversion (historically predicts recession)
    std::vector<FREDObservation> calculateInvertedYieldCurve(int numDays = 10);

private:
    std::string apiKey_;
    static const std::string BASE_URL;

    // Helper function for cURL callbacks
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output);
};

#endif /* FREDDataClient_hpp */
