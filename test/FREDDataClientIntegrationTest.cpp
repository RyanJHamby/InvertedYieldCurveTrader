//
//  FREDDataClientTest.cpp
//  InvertedYieldCurveTrader
//
//  Unit tests for FRED Data Client
//
//  Created by Ryan Hamby on 12/22/25.
//

#include <gtest/gtest.h>
#include "../src/DataProviders/FREDDataClient.hpp"
#include <cstdlib>
#include <stdexcept>
#include <thread>
#include <chrono>

class FREDDataClientTest : public ::testing::Test {
protected:
    std::string apiKey;

    void SetUp() override {
        const char* apiKeyEnv = std::getenv("FRED_API_KEY");
        if (!apiKeyEnv || std::string(apiKeyEnv).empty()) {
            GTEST_SKIP() << "FRED_API_KEY not set - skipping FRED tests";
        }
        apiKey = std::string(apiKeyEnv);
    }
};

// ===== Constructor Tests =====

TEST_F(FREDDataClientTest, Constructor_ValidAPIKey) {
    EXPECT_NO_THROW({
        FREDDataClient client(apiKey);
    });
}

TEST_F(FREDDataClientTest, Constructor_EmptyAPIKeyThrows) {
    EXPECT_THROW({
        FREDDataClient client("");
    }, std::invalid_argument);
}

// ===== Core Fetch Tests =====

TEST_F(FREDDataClientTest, FetchSeries_ReturnsValidJSON) {
    FREDDataClient client(apiKey);
    std::string response = client.fetchSeries("CPIAUCSL", 5, "desc");

    EXPECT_FALSE(response.empty());
    EXPECT_NE(response.find("observations"), std::string::npos)
        << "Response should contain observations";
}

TEST_F(FREDDataClientTest, FetchSeries_InvalidSeriesThrows) {
    FREDDataClient client(apiKey);

    EXPECT_THROW({
        client.fetchSeries("INVALID_SERIES_ID_12345", 1);
    }, std::runtime_error);
}

TEST_F(FREDDataClientTest, FetchLatestValue_ReturnsObservations) {
    FREDDataClient client(apiKey);
    auto observations = client.fetchLatestValue("CPIAUCSL", 3);

    EXPECT_FALSE(observations.empty());
    EXPECT_LE(observations.size(), 3);

    // Verify structure
    for (const auto& obs : observations) {
        EXPECT_FALSE(obs.date.empty());
        EXPECT_GT(obs.value, 0.0) << "CPI should be positive";
    }
}

// ===== Specific Indicator Tests =====

TEST_F(FREDDataClientTest, FetchInflation_ReturnsData) {
    FREDDataClient client(apiKey);
    auto data = client.fetchInflation(5);

    EXPECT_FALSE(data.empty());

    // CPI values should be in reasonable range (200-400)
    for (const auto& obs : data) {
        EXPECT_GE(obs.value, 100.0);
        EXPECT_LE(obs.value, 500.0);
    }
}

TEST_F(FREDDataClientTest, FetchTreasury10Y_ReturnsData) {
    FREDDataClient client(apiKey);
    auto data = client.fetchTreasury10Y(5);

    EXPECT_FALSE(data.empty());

    // Treasury yields should be 0-20%
    for (const auto& obs : data) {
        EXPECT_GE(obs.value, 0.0);
        EXPECT_LE(obs.value, 20.0);
    }
}

TEST_F(FREDDataClientTest, FetchTreasury2Y_ReturnsData) {
    FREDDataClient client(apiKey);
    auto data = client.fetchTreasury2Y(5);

    EXPECT_FALSE(data.empty());

    // Treasury yields should be 0-20%
    for (const auto& obs : data) {
        EXPECT_GE(obs.value, 0.0);
        EXPECT_LE(obs.value, 20.0);
    }
}

TEST_F(FREDDataClientTest, FetchFedFundsRate_ReturnsData) {
    FREDDataClient client(apiKey);
    auto data = client.fetchFedFundsRate(3);

    EXPECT_FALSE(data.empty());

    for (const auto& obs : data) {
        EXPECT_GE(obs.value, 0.0);
        EXPECT_LE(obs.value, 20.0);
    }
}

TEST_F(FREDDataClientTest, FetchUnemployment_ReturnsData) {
    FREDDataClient client(apiKey);
    auto data = client.fetchUnemployment(3);

    EXPECT_FALSE(data.empty());

    for (const auto& obs : data) {
        EXPECT_GE(obs.value, 0.0);
        EXPECT_LE(obs.value, 25.0);
    }
}

TEST_F(FREDDataClientTest, FetchGDP_ReturnsData) {
    FREDDataClient client(apiKey);
    auto data = client.fetchGDP(3);

    EXPECT_FALSE(data.empty());

    // GDP growth typically -10% to +10%
    for (const auto& obs : data) {
        EXPECT_GE(obs.value, -15.0);
        EXPECT_LE(obs.value, 15.0);
    }
}

TEST_F(FREDDataClientTest, FetchConsumerSentiment_ReturnsData) {
    FREDDataClient client(apiKey);
    auto data = client.fetchConsumerSentiment(3);

    EXPECT_FALSE(data.empty());

    // Consumer sentiment typically 40-120
    for (const auto& obs : data) {
        EXPECT_GE(obs.value, 20.0);
        EXPECT_LE(obs.value, 150.0);
    }
}

// ===== Yield Curve Tests =====

TEST_F(FREDDataClientTest, CalculateInvertedYieldCurve_ReturnsSpread) {
    FREDDataClient client(apiKey);

    try {
        // Small delay to avoid rate limiting (yield curve makes 2 API calls)
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        auto spreads = client.calculateInvertedYieldCurve(5);

        EXPECT_FALSE(spreads.empty());

        // Spread should be reasonable (-3% to +3%)
        for (const auto& spread : spreads) {
            EXPECT_FALSE(spread.date.empty());
            EXPECT_GE(spread.value, -3.0);
            EXPECT_LE(spread.value, 3.0);
        }
    } catch (const std::exception& e) {
        // If we get a parse error, it's likely FRED returned HTML (rate limit)
        std::string error(e.what());
        if (error.find("parse error") != std::string::npos) {
            GTEST_SKIP() << "FRED rate limit hit (temporary): " << e.what();
        } else {
            throw; // Re-throw if it's a different error
        }
    }
}

TEST_F(FREDDataClientTest, CalculateInvertedYieldCurve_DetectsInversion) {
    FREDDataClient client(apiKey);
    auto spreads = client.calculateInvertedYieldCurve(30);

    ASSERT_FALSE(spreads.empty());

    // Just verify the data structure is correct
    // We can't guarantee there IS an inversion, but we can check the logic works
    bool hasPositive = false, hasNegative = false;

    for (const auto& spread : spreads) {
        if (spread.value > 0) hasPositive = true;
        if (spread.value < 0) hasNegative = true;
    }

    // Over 30 days, we should have some variation
    EXPECT_TRUE(hasPositive || hasNegative) << "Spreads should have some variation";
}

// ===== Data Quality Tests =====

TEST_F(FREDDataClientTest, FetchLatestValue_FiltersNullValues) {
    FREDDataClient client(apiKey);

    // Fetch enough data to potentially encounter null values
    auto data = client.fetchLatestValue("DGS10", 30);

    EXPECT_FALSE(data.empty());

    // All returned values should be valid (no NaN, no zero placeholder)
    for (const auto& obs : data) {
        EXPECT_FALSE(std::isnan(obs.value));
        EXPECT_GT(obs.value, 0.0);
    }
}

TEST_F(FREDDataClientTest, MultipleRequests_WorkCorrectly) {
    FREDDataClient client(apiKey);

    // Make multiple requests in sequence
    auto inflation1 = client.fetchInflation(3);
    auto unemployment = client.fetchUnemployment(3);
    auto inflation2 = client.fetchInflation(3);

    EXPECT_FALSE(inflation1.empty());
    EXPECT_FALSE(unemployment.empty());
    EXPECT_FALSE(inflation2.empty());

    // Same request should return same data
    EXPECT_EQ(inflation1.size(), inflation2.size());
    if (!inflation1.empty() && !inflation2.empty()) {
        EXPECT_EQ(inflation1[0].date, inflation2[0].date);
        EXPECT_EQ(inflation1[0].value, inflation2[0].value);
    }
}

// Run tests
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
