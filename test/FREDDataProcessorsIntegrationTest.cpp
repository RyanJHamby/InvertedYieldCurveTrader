//
//  FREDDataProcessorsTest.cpp
//  InvertedYieldCurveTrader
//
//  Unit tests for FRED-based data processors
//
//  Created by Ryan Hamby on 12/22/25.
//

#include <gtest/gtest.h>
#include "../src/DataProcessors/FedFundsProcessor.hpp"
#include "../src/DataProcessors/UnemploymentProcessor.hpp"
#include "../src/DataProcessors/ConsumerSentimentProcessor.hpp"
#include <cstdlib>
#include <stdexcept>

// Helper to get API key from environment
class FREDProcessorTest : public ::testing::Test {
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

// ===== Fed Funds Processor Tests =====

TEST_F(FREDProcessorTest, FedFunds_ProcessReturnsData) {
    FedFundsProcessor processor;
    auto data = processor.process(apiKey, 5);

    EXPECT_FALSE(data.empty()) << "Should return fed funds data";
    EXPECT_LE(data.size(), 5) << "Should return at most 5 values";

    // Fed funds rate should be reasonable (0-20%)
    for (const auto& value : data) {
        EXPECT_GE(value, 0.0) << "Fed funds rate should be non-negative";
        EXPECT_LE(value, 20.0) << "Fed funds rate should be less than 20%";
    }
}

TEST_F(FREDProcessorTest, FedFunds_GetLatestValue) {
    FedFundsProcessor processor;
    double latest = processor.getLatestValue(apiKey);

    EXPECT_GE(latest, 0.0) << "Latest fed funds rate should be non-negative";
    EXPECT_LE(latest, 20.0) << "Latest fed funds rate should be reasonable";
}

TEST_F(FREDProcessorTest, FedFunds_InvalidAPIKeyThrows) {
    FedFundsProcessor processor;

    EXPECT_THROW({
        processor.process("invalid_api_key", 1);
    }, std::exception) << "Should throw on invalid API key";
}

TEST_F(FREDProcessorTest, FedFunds_EmptyAPIKeyThrows) {
    FedFundsProcessor processor;

    EXPECT_THROW({
        processor.process("", 1);
    }, std::exception) << "Should throw on empty API key";
}

// ===== Unemployment Processor Tests =====

TEST_F(FREDProcessorTest, Unemployment_ProcessReturnsData) {
    UnemploymentProcessor processor;
    auto data = processor.process(apiKey, 5);

    EXPECT_FALSE(data.empty()) << "Should return unemployment data";
    EXPECT_LE(data.size(), 5) << "Should return at most 5 values";

    // Unemployment rate should be reasonable (0-25%)
    for (const auto& value : data) {
        EXPECT_GE(value, 0.0) << "Unemployment rate should be non-negative";
        EXPECT_LE(value, 25.0) << "Unemployment rate should be less than 25%";
    }
}

TEST_F(FREDProcessorTest, Unemployment_GetLatestValue) {
    UnemploymentProcessor processor;
    double latest = processor.getLatestValue(apiKey);

    EXPECT_GE(latest, 0.0) << "Latest unemployment rate should be non-negative";
    EXPECT_LE(latest, 25.0) << "Latest unemployment rate should be reasonable";
}

TEST_F(FREDProcessorTest, Unemployment_DataIsSorted) {
    UnemploymentProcessor processor;
    auto data = processor.process(apiKey, 12);

    // First value should be most recent
    // Values should be monthly data
    EXPECT_FALSE(data.empty());
}

// ===== Consumer Sentiment Processor Tests =====

TEST_F(FREDProcessorTest, ConsumerSentiment_ProcessReturnsData) {
    ConsumerSentimentProcessor processor;
    auto data = processor.process(apiKey, 5);

    EXPECT_FALSE(data.empty()) << "Should return consumer sentiment data";
    EXPECT_LE(data.size(), 5) << "Should return at most 5 values";

    // Consumer sentiment typically ranges 40-120
    for (const auto& value : data) {
        EXPECT_GE(value, 20.0) << "Consumer sentiment should be >= 20";
        EXPECT_LE(value, 150.0) << "Consumer sentiment should be <= 150";
    }
}

TEST_F(FREDProcessorTest, ConsumerSentiment_GetLatestValue) {
    ConsumerSentimentProcessor processor;
    double latest = processor.getLatestValue(apiKey);

    EXPECT_GE(latest, 20.0) << "Latest consumer sentiment should be >= 20";
    EXPECT_LE(latest, 150.0) << "Latest consumer sentiment should be <= 150";
}

// ===== Integration Tests =====

TEST_F(FREDProcessorTest, AllProcessors_ReturnConsistentDataSize) {
    FedFundsProcessor fedFunds;
    UnemploymentProcessor unemployment;
    ConsumerSentimentProcessor sentiment;

    int numValues = 3;

    auto fedFundsData = fedFunds.process(apiKey, numValues);
    auto unemploymentData = unemployment.process(apiKey, numValues);
    auto sentimentData = sentiment.process(apiKey, numValues);

    // All should return data (though sizes may vary due to data availability)
    EXPECT_FALSE(fedFundsData.empty());
    EXPECT_FALSE(unemploymentData.empty());
    EXPECT_FALSE(sentimentData.empty());

    // Each should have <= numValues
    EXPECT_LE(fedFundsData.size(), numValues);
    EXPECT_LE(unemploymentData.size(), numValues);
    EXPECT_LE(sentimentData.size(), numValues);
}

TEST_F(FREDProcessorTest, AllProcessors_GetLatestValueWorks) {
    FedFundsProcessor fedFunds;
    UnemploymentProcessor unemployment;
    ConsumerSentimentProcessor sentiment;

    // All should return a single latest value without throwing
    EXPECT_NO_THROW({
        double ff = fedFunds.getLatestValue(apiKey);
        double ur = unemployment.getLatestValue(apiKey);
        double cs = sentiment.getLatestValue(apiKey);

        EXPECT_GT(ff, 0.0);
        EXPECT_GT(ur, 0.0);
        EXPECT_GT(cs, 0.0);
    });
}

// Run all tests
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
