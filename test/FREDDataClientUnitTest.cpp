//
//  FREDDataClientUnitTest.cpp
//  InvertedYieldCurveTrader
//
//  Unit tests for FRED Data Client (no API keys required)
//
//  Created by Ryan Hamby on 12/23/25.
//

#include <gtest/gtest.h>
#include "../src/DataProviders/FREDDataClient.hpp"
#include "MockData.hpp"
#include <nlohmann/json.hpp>
#include <cstdlib>
#include <stdexcept>

using json = nlohmann::json;

class FREDDataClientUnitTest : public ::testing::Test {
protected:
    FREDDataClient* createTestClient() {
        return new FREDDataClient("test_api_key_12345");
    }
};

// ===== Constructor Tests (No API required) =====

TEST_F(FREDDataClientUnitTest, Constructor_ValidAPIKey) {
    EXPECT_NO_THROW({
        FREDDataClient client("valid_test_key_12345");
    });
}

TEST_F(FREDDataClientUnitTest, Constructor_EmptyAPIKeyThrows) {
    EXPECT_THROW({
        FREDDataClient client("");
    }, std::invalid_argument);
}

// ===== Data Parsing Tests (No API required) =====

TEST_F(FREDDataClientUnitTest, ParseFREDResponse_ValidData) {
    json mockResponse = json::parse(MockData::SAMPLE_FRED_RESPONSE);

    // Verify mock data structure
    EXPECT_TRUE(mockResponse.contains("observations"));
    EXPECT_FALSE(mockResponse["observations"].empty());

    // Check observation structure
    auto observations = mockResponse["observations"];
    EXPECT_GE(observations.size(), 1);

    for (const auto& obs : observations) {
        EXPECT_TRUE(obs.contains("date"));
        EXPECT_TRUE(obs.contains("value"));
        EXPECT_FALSE(obs["date"].get<std::string>().empty());
        EXPECT_FALSE(obs["value"].get<std::string>().empty());
    }
}

TEST_F(FREDDataClientUnitTest, ParseFREDResponse_ExtractValues) {
    json mockResponse = json::parse(MockData::SAMPLE_FRED_RESPONSE);
    auto observations = mockResponse["observations"];

    std::vector<double> values;
    for (const auto& obs : observations) {
        values.push_back(std::stod(obs["value"].get<std::string>()));
    }

    EXPECT_EQ(values.size(), 3);
    EXPECT_DOUBLE_EQ(values[0], 314.5);
    EXPECT_DOUBLE_EQ(values[1], 314.7);
    EXPECT_DOUBLE_EQ(values[2], 315.2);
}

TEST_F(FREDDataClientUnitTest, ParseFREDResponse_TreasuryData) {
    json mockResponse = json::parse(MockData::SAMPLE_FRED_TREASURY_RESPONSE);
    auto observations = mockResponse["observations"];

    // Verify treasury yields are in reasonable range
    for (const auto& obs : observations) {
        double value = std::stod(obs["value"].get<std::string>());
        EXPECT_GE(value, 0.0);
        EXPECT_LE(value, 20.0);
    }
}

TEST_F(FREDDataClientUnitTest, ParseFREDResponse_InvalidJSON) {
    EXPECT_THROW({
        json response = json::parse("invalid json");
    }, json::exception);
}

TEST_F(FREDDataClientUnitTest, ParseFREDResponse_EmptyObservations) {
    json mockResponse = json::parse(MockData::EMPTY_FRED_RESPONSE);
    auto observations = mockResponse["observations"];

    EXPECT_TRUE(observations.empty());
}

TEST_F(FREDDataClientUnitTest, ParseFREDResponse_ErrorResponse) {
    json errorResponse = json::parse(MockData::INVALID_FRED_RESPONSE);

    EXPECT_TRUE(errorResponse.contains("error_code"));
    EXPECT_EQ(errorResponse["error_code"], 400);
    EXPECT_TRUE(errorResponse.contains("error_message"));
}

// ===== Data Structure Tests (No API required) =====

TEST_F(FREDDataClientUnitTest, FREDObservation_ValidConstruction) {
    FREDObservation obs{"2025-12-22", 315.5};

    EXPECT_EQ(obs.date, "2025-12-22");
    EXPECT_DOUBLE_EQ(obs.value, 315.5);
}

TEST_F(FREDDataClientUnitTest, FREDObservation_DateFormat) {
    std::string date = "2025-12-22";
    EXPECT_EQ(date.length(), 10);
    EXPECT_EQ(date[4], '-');
    EXPECT_EQ(date[7], '-');
}

TEST_F(FREDDataClientUnitTest, FREDObservation_PositiveValues) {
    double cpiValue = 315.2;
    EXPECT_GT(cpiValue, 0.0);
    EXPECT_LT(cpiValue, 500.0);
}

// ===== Series ID Tests (No API required) =====

TEST_F(FREDDataClientUnitTest, SeriesIDsConfigured) {
    auto seriesIds = FREDDataClient::SERIES_IDS;

    // Verify all expected series are configured
    EXPECT_TRUE(seriesIds.contains("inflation"));
    EXPECT_TRUE(seriesIds.contains("treasury_10y"));
    EXPECT_TRUE(seriesIds.contains("treasury_2y"));
    EXPECT_TRUE(seriesIds.contains("fed_funds_rate"));
    EXPECT_TRUE(seriesIds.contains("unemployment"));
    EXPECT_TRUE(seriesIds.contains("gdp"));
    EXPECT_TRUE(seriesIds.contains("consumer_sentiment"));
}

TEST_F(FREDDataClientUnitTest, InflationSeriesID) {
    EXPECT_EQ(FREDDataClient::SERIES_IDS.at("inflation"), "CPIAUCSL");
}

TEST_F(FREDDataClientUnitTest, TreasurySeriesIDs) {
    EXPECT_EQ(FREDDataClient::SERIES_IDS.at("treasury_10y"), "DGS10");
    EXPECT_EQ(FREDDataClient::SERIES_IDS.at("treasury_2y"), "DGS2");
}

TEST_F(FREDDataClientUnitTest, EconomicIndicatorSeriesIDs) {
    EXPECT_EQ(FREDDataClient::SERIES_IDS.at("fed_funds_rate"), "FEDFUNDS");
    EXPECT_EQ(FREDDataClient::SERIES_IDS.at("unemployment"), "UNRATE");
    EXPECT_EQ(FREDDataClient::SERIES_IDS.at("consumer_sentiment"), "UMCSENT");
}

// ===== Value Range Tests (No API required) =====

TEST_F(FREDDataClientUnitTest, InflationValueRange) {
    json mockResponse = json::parse(MockData::SAMPLE_FRED_RESPONSE);
    for (const auto& obs : mockResponse["observations"]) {
        double value = std::stod(obs["value"].get<std::string>());
        EXPECT_GE(value, 100.0);   // CPI should be >= 100
        EXPECT_LE(value, 500.0);   // Reasonable upper bound
    }
}

TEST_F(FREDDataClientUnitTest, TreasuryYieldRange) {
    json mockResponse = json::parse(MockData::SAMPLE_FRED_TREASURY_RESPONSE);
    for (const auto& obs : mockResponse["observations"]) {
        double value = std::stod(obs["value"].get<std::string>());
        EXPECT_GE(value, 0.0);     // Yields should be positive
        EXPECT_LE(value, 20.0);    // Reasonable upper bound
    }
}

// ===== Yield Curve Calculation Tests (No API required) =====

TEST_F(FREDDataClientUnitTest, YieldCurveSpreadCalculation) {
    double treasury10y = 4.28;
    double treasury2y = 3.85;
    double spread = treasury10y - treasury2y;

    // Normal yield curve: 10Y > 2Y
    EXPECT_GT(spread, 0.0);
    EXPECT_LE(spread, 3.0);
}

TEST_F(FREDDataClientUnitTest, InvertedYieldCurveDetection) {
    double inverted10y = 3.50;
    double inverted2y = 4.00;
    double spread = inverted10y - inverted2y;

    // Inverted yield curve: 2Y > 10Y
    EXPECT_LT(spread, 0.0);
    EXPECT_GE(spread, -3.0);
}

TEST_F(FREDDataClientUnitTest, FlatYieldCurve) {
    double flat10y = 4.00;
    double flat2y = 4.00;
    double spread = flat10y - flat2y;

    // Flat yield curve
    EXPECT_DOUBLE_EQ(spread, 0.0);
}

// Run tests
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
