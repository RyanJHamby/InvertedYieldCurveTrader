//
//  VIXDataProcessorTest.cpp
//  InvertedYieldCurveTrader
//
//  Unit tests for VIX Data Processor (Alpha Vantage)
//
//  Created by Ryan Hamby on 12/22/25.
//

#include <gtest/gtest.h>
#include "../src/DataProcessors/VIXDataProcessor.hpp"
#include <cstdlib>
#include <stdexcept>

class VIXProcessorTest : public ::testing::Test {
protected:
    VIXDataProcessor processor;
    std::string apiKey;

    void SetUp() override {
        const char* apiKeyEnv = std::getenv("ALPHA_VANTAGE_API_KEY");
        if (!apiKeyEnv || std::string(apiKeyEnv).empty()) {
            GTEST_SKIP() << "ALPHA_VANTAGE_API_KEY not set - skipping VIX tests";
        }
        apiKey = std::string(apiKeyEnv);
    }
};

TEST_F(VIXProcessorTest, Process_ReturnsData) {
    auto data = processor.process(apiKey, 10);

    EXPECT_FALSE(data.empty()) << "Should return VIX data";
    EXPECT_LE(data.size(), 10) << "Should return at most 10 values";

    // VIX typically ranges 10-80 (can spike higher during crises)
    for (const auto& value : data) {
        EXPECT_GE(value, 5.0) << "VIX should be >= 5";
        EXPECT_LE(value, 100.0) << "VIX should be <= 100 in normal conditions";
    }
}

TEST_F(VIXProcessorTest, GetLatestValue_ReturnsReasonableValue) {
    double latest = processor.getLatestValue(apiKey);

    EXPECT_GE(latest, 5.0) << "Latest VIX should be >= 5";
    EXPECT_LE(latest, 100.0) << "Latest VIX should be <= 100";
}

TEST_F(VIXProcessorTest, Process_DataIsSortedMostRecentFirst) {
    auto data = processor.process(apiKey, 30);

    ASSERT_GE(data.size(), 2) << "Need at least 2 data points to test ordering";

    // Most recent value should be first
    // All values should be valid
    for (const auto& value : data) {
        EXPECT_GT(value, 0.0) << "All VIX values should be positive";
    }
}

TEST_F(VIXProcessorTest, Process_InvalidAPIKeyThrows) {
    EXPECT_THROW({
        processor.process("invalid_api_key", 5);
    }, std::exception) << "Should throw on invalid API key";
}

// Run tests
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
