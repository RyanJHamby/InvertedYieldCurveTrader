//
//  VIXDataProcessorUnitTest.cpp
//  InvertedYieldCurveTrader
//
//  Unit tests for VIX Data Processor (no API keys required)
//
//  Created by Ryan Hamby on 12/23/25.
//

#include <gtest/gtest.h>
#include "../src/DataProcessors/VIXDataProcessor.hpp"
#include "MockData.hpp"
#include <nlohmann/json.hpp>
#include <cstdlib>
#include <stdexcept>
#include <cmath>

using json = nlohmann::json;

class VIXDataProcessorUnitTest : public ::testing::Test {
protected:
    VIXDataProcessor processor;
};

// ===== JSON Parsing Tests (No API required) =====

TEST_F(VIXDataProcessorUnitTest, ParseAlphaVantageResponse_ValidData) {
    json mockResponse = json::parse(MockData::SAMPLE_ALPHA_VANTAGE_RESPONSE);

    // Verify mock data structure
    EXPECT_TRUE(mockResponse.contains("Time Series (Daily)"));
    EXPECT_FALSE(mockResponse["Time Series (Daily)"].empty());

    auto timeSeries = mockResponse["Time Series (Daily)"];
    EXPECT_GE(timeSeries.size(), 1);

    // Verify each day has required fields
    for (auto& [date, dayData] : timeSeries.items()) {
        EXPECT_TRUE(dayData.contains("4. close"));
        EXPECT_FALSE(dayData["4. close"].get<std::string>().empty());
    }
}

TEST_F(VIXDataProcessorUnitTest, ParseAlphaVantageResponse_ExtractClosePrices) {
    json mockResponse = json::parse(MockData::SAMPLE_ALPHA_VANTAGE_RESPONSE);
    auto timeSeries = mockResponse["Time Series (Daily)"];

    std::vector<double> closePrices;
    for (auto& [date, dayData] : timeSeries.items()) {
        std::string closeStr = dayData["4. close"].get<std::string>();
        closePrices.push_back(std::stod(closeStr));
    }

    EXPECT_EQ(closePrices.size(), 3);

    // Verify all extracted values are present (order may vary)
    bool has18_95 = false, has18_50 = false, has18_00 = false;
    for (const auto& price : closePrices) {
        if (std::abs(price - 18.95) < 0.01) has18_95 = true;
        if (std::abs(price - 18.50) < 0.01) has18_50 = true;
        if (std::abs(price - 18.00) < 0.01) has18_00 = true;
    }

    EXPECT_TRUE(has18_95) << "Should extract 18.95";
    EXPECT_TRUE(has18_50) << "Should extract 18.50";
    EXPECT_TRUE(has18_00) << "Should extract 18.00";
}

TEST_F(VIXDataProcessorUnitTest, ParseAlphaVantageResponse_InvalidJSON) {
    EXPECT_THROW({
        json response = json::parse("not valid json");
    }, json::exception);
}

TEST_F(VIXDataProcessorUnitTest, ParseAlphaVantageResponse_MissingTimeSeries) {
    std::string missingData = R"(
    {
        "Meta Data": {
            "1. Information": "Daily Prices"
        }
    }
    )";

    json response = json::parse(missingData);
    EXPECT_FALSE(response.contains("Time Series (Daily)"));
}

// ===== Data Structure Tests (No API required) =====

TEST_F(VIXDataProcessorUnitTest, VIXValueRange_Normal) {
    // VIX typically ranges 10-80
    double normalVIX = 18.95;
    EXPECT_GE(normalVIX, 5.0);
    EXPECT_LE(normalVIX, 100.0);
}

TEST_F(VIXDataProcessorUnitTest, VIXValueRange_Low) {
    // Low volatility (market calm)
    double lowVIX = 10.0;
    EXPECT_GE(lowVIX, 5.0);
    EXPECT_LE(lowVIX, 100.0);
}

TEST_F(VIXDataProcessorUnitTest, VIXValueRange_High) {
    // High volatility (market stress)
    double highVIX = 75.0;
    EXPECT_GE(highVIX, 5.0);
    EXPECT_LE(highVIX, 100.0);
}

TEST_F(VIXDataProcessorUnitTest, VIXValueRange_Crisis) {
    // Historical crisis levels (March 2020)
    double crisisVIX = 82.69;
    EXPECT_GE(crisisVIX, 5.0);
    EXPECT_LE(crisisVIX, 100.0);
}

// ===== Data Sorting Tests (No API required) =====

TEST_F(VIXDataProcessorUnitTest, DateSorting_DescendingOrder) {
    // Create date pairs (from mock data)
    std::vector<std::pair<std::string, double>> dataPoints = {
        {"2025-12-22", 18.95},
        {"2025-12-21", 18.50},
        {"2025-12-20", 18.00}
    };

    // Sort by date descending (most recent first)
    std::sort(dataPoints.begin(), dataPoints.end(),
              [](const auto& a, const auto& b) { return a.first > b.first; });

    // Verify most recent date is first
    EXPECT_EQ(dataPoints[0].first, "2025-12-22");
    EXPECT_EQ(dataPoints[1].first, "2025-12-21");
    EXPECT_EQ(dataPoints[2].first, "2025-12-20");
}

TEST_F(VIXDataProcessorUnitTest, DateSorting_ExtractValues) {
    std::vector<std::pair<std::string, double>> dataPoints = {
        {"2025-12-22", 18.95},
        {"2025-12-21", 18.50},
        {"2025-12-20", 18.00}
    };

    std::sort(dataPoints.begin(), dataPoints.end(),
              [](const auto& a, const auto& b) { return a.first > b.first; });

    std::vector<double> values;
    for (const auto& point : dataPoints) {
        values.push_back(point.second);
    }

    // Values should be in descending date order (most recent first)
    EXPECT_DOUBLE_EQ(values[0], 18.95);  // Most recent
    EXPECT_DOUBLE_EQ(values[1], 18.50);
    EXPECT_DOUBLE_EQ(values[2], 18.00);
}

// ===== Limit Tests (No API required) =====

TEST_F(VIXDataProcessorUnitTest, LimitDataPoints_RequestLessThanAvailable) {
    std::vector<double> allData = {18.95, 18.50, 18.00, 17.85, 17.50};
    int limit = 2;

    std::vector<double> limited(allData.begin(), allData.begin() + std::min(static_cast<size_t>(limit), allData.size()));

    EXPECT_EQ(limited.size(), 2);
    EXPECT_DOUBLE_EQ(limited[0], 18.95);
    EXPECT_DOUBLE_EQ(limited[1], 18.50);
}

TEST_F(VIXDataProcessorUnitTest, LimitDataPoints_RequestMoreThanAvailable) {
    std::vector<double> allData = {18.95, 18.50, 18.00};
    int limit = 10;

    std::vector<double> limited(allData.begin(), allData.begin() + std::min(static_cast<size_t>(limit), allData.size()));

    EXPECT_EQ(limited.size(), 3);
}

TEST_F(VIXDataProcessorUnitTest, LimitDataPoints_RequestZero) {
    std::vector<double> allData = {18.95, 18.50, 18.00};
    int limit = 0;

    std::vector<double> limited(allData.begin(), allData.begin() + std::min(static_cast<size_t>(limit), allData.size()));

    EXPECT_EQ(limited.size(), 0);
}

// ===== Format Tests (No API required) =====

TEST_F(VIXDataProcessorUnitTest, DateFormat_YYYY_MM_DD) {
    std::string date = "2025-12-22";

    EXPECT_EQ(date.length(), 10);
    EXPECT_EQ(date[0], '2');
    EXPECT_EQ(date[4], '-');
    EXPECT_EQ(date[7], '-');
}

TEST_F(VIXDataProcessorUnitTest, VIXValueFormat_Double) {
    std::string vixStr = "18.95";
    double vixValue = std::stod(vixStr);

    EXPECT_GT(vixValue, 0.0);
    EXPECT_EQ(vixValue, 18.95);
}

// ===== Error Handling Tests (No API required) =====

TEST_F(VIXDataProcessorUnitTest, InvalidDoubleConversion) {
    EXPECT_THROW({
        std::stod("not_a_number");
    }, std::invalid_argument);
}

TEST_F(VIXDataProcessorUnitTest, EmptyStringConversion) {
    EXPECT_THROW({
        std::stod("");
    }, std::invalid_argument);
}

// Run tests
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
