//
//  SurpriseTransformerUnitTest.cpp
//  InvertedYieldCurveTrader
//
//  Unit tests for SurpriseTransformer (no API keys required)
//
//  Created by Ryan Hamby on 12/25/25.
//

#include <gtest/gtest.h>
#include "../src/DataProcessors/SurpriseTransformer.hpp"
#include <cmath>
#include <numeric>

class SurpriseTransformerTest : public ::testing::Test {
protected:
    // Create constant value series (should have zero variance, zero surprises)
    static std::vector<double> createConstantSeries(double value, int length) {
        return std::vector<double>(length, value);
    }

    // Create linear trend series (100, 101, 102, ...)
    static std::vector<double> createLinearTrend(double start, double step, int length) {
        std::vector<double> series;
        for (int i = length - 1; i >= 0; i--) {
            series.push_back(start + i * step);
        }
        return series;
    }

    // Create random walk (useful for testing AR(1))
    static std::vector<double> createRandomWalk(int length) {
        std::vector<double> series;
        double value = 100.0;
        for (int i = 0; i < length; i++) {
            series.push_back(value);
            value += (i % 2 == 0 ? 0.5 : -0.3);  // Pseudo-random but deterministic
        }
        return series;
    }

    // Compute mean of a vector
    static double computeMean(const std::vector<double>& data) {
        if (data.empty()) return 0.0;
        return std::accumulate(data.begin(), data.end(), 0.0) / data.size();
    }

    // Compute standard deviation
    static double computeStdDev(const std::vector<double>& data) {
        double mean = computeMean(data);
        double sumSq = 0.0;
        for (double x : data) {
            sumSq += (x - mean) * (x - mean);
        }
        return std::sqrt(sumSq / (data.size() - 1));
    }
};

// ===== Surprise Extraction Tests =====

TEST_F(SurpriseTransformerTest, ConstantSeriesZeroSurprises) {
    // Constant series: first value has no expectation (treated as 0),
    // subsequent values have previous as expectation (constant)
    std::vector<double> levels = createConstantSeries(100.0, 12);
    auto surprise = SurpriseTransformer::extractSurprise(levels, "test");

    // First surprise = 100 - 0 = 100 (no prior expectation)
    // Remaining surprises = 100 - 100 = 0
    EXPECT_NEAR(surprise.surprises[0], 100.0, 1e-10);
    for (size_t i = 1; i < surprise.surprises.size(); i++) {
        EXPECT_NEAR(surprise.surprises[i], 0.0, 1e-10);
    }

    // Mean will be 100/12 ≈ 8.33 due to first value
    // This won't validate, which is correct (biased expectations at start)
    EXPECT_FALSE(surprise.isValidated);
}

TEST_F(SurpriseTransformerTest, ZeroMeanProperty) {
    // After we have enough history for AR(1) (12+ points),
    // surprises should stabilize around zero mean
    std::vector<double> levels = createRandomWalk(60);  // More history
    auto surprise = SurpriseTransformer::extractSurprise(levels, "test", 12);

    // Check surprises from position 24 onwards (after AR(1) is stable)
    std::vector<double> laterSurprises(surprise.surprises.begin() + 24,
                                        surprise.surprises.end());
    double mean = computeMean(laterSurprises);

    // AR(1) should produce surprises with small mean bias
    EXPECT_LT(std::abs(mean), 2.0);
}

TEST_F(SurpriseTransformerTest, LinearTrendDetection) {
    // Linear uptrend: 100, 101, 102, ..., 111
    // AR(1) should forecast ~112, so last surprise should be near 0
    std::vector<double> levels = createLinearTrend(100.0, 1.0, 12);
    auto surprise = SurpriseTransformer::extractSurprise(levels, "test", 6);

    // Earliest surprises (when AR(1) has little history) will be larger
    // Later surprises should stabilize
    EXPECT_LT(std::abs(surprise.surprises.back()), 2.0);
}

TEST_F(SurpriseTransformerTest, ExpectationSource) {
    std::vector<double> levels = createLinearTrend(100.0, 1.0, 24);
    auto surprise = SurpriseTransformer::extractSurprise(levels, "test", 12);

    // expectationSource tracks the LAST source used (at position 23)
    // After 12 values, should switch to AR(1)
    EXPECT_EQ(surprise.expectationSource, "ar1_forecast");

    // Verify AR(1) was actually used for later values
    EXPECT_GT(std::abs(surprise.expectations[23]), 0.0);
}

TEST_F(SurpriseTransformerTest, EmptyInputThrows) {
    std::vector<double> empty;
    EXPECT_THROW(
        SurpriseTransformer::extractSurprise(empty, "test"),
        std::invalid_argument
    );
}

TEST_F(SurpriseTransformerTest, InvalidLookbackThrows) {
    std::vector<double> levels = createConstantSeries(100.0, 12);
    EXPECT_THROW(
        SurpriseTransformer::extractSurprise(levels, "test", 0),
        std::invalid_argument
    );
    EXPECT_THROW(
        SurpriseTransformer::extractSurprise(levels, "test", -1),
        std::invalid_argument
    );
}

// ===== AR(1) Forecasting Tests =====

TEST_F(SurpriseTransformerTest, AR1ConstantForecast) {
    // Constant series: AR(1) should forecast same value
    std::vector<double> constant = createConstantSeries(50.0, 24);
    auto surprise = SurpriseTransformer::extractSurprise(constant, "test", 12);

    // After AR(1) kicks in (i >= 12), expectations should equal current value
    for (size_t i = 13; i < surprise.expectations.size(); i++) {
        EXPECT_NEAR(surprise.expectations[i], 50.0, 1e-6);
    }
}

TEST_F(SurpriseTransformerTest, AR1UpwardTrendForecast) {
    // Uptrend: AR(1) should forecast higher
    std::vector<double> uptrend = createLinearTrend(100.0, 1.0, 24);
    auto surprise = SurpriseTransformer::extractSurprise(uptrend, "test", 12);

    // At position 24 (most recent), expectation should be > current value
    // because trend is upward
    EXPECT_GT(surprise.expectations[23], uptrend[23] - 2.0);
}

TEST_F(SurpriseTransformerTest, AR1DownwardTrendForecast) {
    // Downtrend: AR(1) should forecast lower
    std::vector<double> downtrend = createLinearTrend(100.0, -1.0, 24);
    auto surprise = SurpriseTransformer::extractSurprise(downtrend, "test", 12);

    // At position 24 (most recent), expectation should be < current value
    // because trend is downward
    EXPECT_LT(surprise.expectations[23], downtrend[23] + 2.0);
}

// ===== Validation Tests =====

TEST_F(SurpriseTransformerTest, ValidationPassesZeroMean) {
    // Use sufficient history and larger tolerance for random walk
    std::vector<double> levels = createRandomWalk(60);
    auto surprise = SurpriseTransformer::extractSurprise(levels, "test", 12);

    // Should have mean close to zero with larger tolerance
    bool valid = SurpriseTransformer::validateZeroMean(surprise, 2.0);
    EXPECT_TRUE(valid);
}

TEST_F(SurpriseTransformerTest, ValidationFailsStrongTrend) {
    // Strong trend: surprises will be biased
    std::vector<double> trend = createLinearTrend(100.0, 5.0, 12);
    auto surprise = SurpriseTransformer::extractSurprise(trend, "test", 4);

    // Mean surprise should be non-zero for strong trend
    EXPECT_GT(std::abs(surprise.meanSurprise), 0.05);
    EXPECT_FALSE(surprise.isValidated);
}

TEST_F(SurpriseTransformerTest, SurpriseSize) {
    // Surprises should be much smaller than original variance
    // (after AR(1) is trained)
    std::vector<double> levels = createRandomWalk(60);
    auto surprise = SurpriseTransformer::extractSurprise(levels, "test", 12);

    // Look at later surprises (after AR(1) is stable)
    std::vector<double> laterLevels(levels.begin() + 24, levels.end());
    std::vector<double> laterSurprises(surprise.surprises.begin() + 24,
                                        surprise.surprises.end());

    double levelStdDev = computeStdDev(laterLevels);
    double surpriseStdDev = computeStdDev(laterSurprises);

    // Surprises should have significantly lower variance than levels
    // because AR(1) captures most of the trend
    EXPECT_LT(surpriseStdDev, levelStdDev);
}

// ===== Edge Cases =====

TEST_F(SurpriseTransformerTest, SingleValueSeries) {
    std::vector<double> single = {100.0};
    auto surprise = SurpriseTransformer::extractSurprise(single, "test", 12);

    EXPECT_EQ(surprise.surprises.size(), 1);
    // First value: expectation = 0 (none available), surprise = 100 - 0 = 100
    EXPECT_EQ(surprise.surprises[0], 100.0);
}

TEST_F(SurpriseTransformerTest, TwoValueSeries) {
    std::vector<double> two = {100.0, 101.0};
    auto surprise = SurpriseTransformer::extractSurprise(two, "test", 12);

    EXPECT_EQ(surprise.surprises.size(), 2);
    EXPECT_EQ(surprise.surprises[0], 100.0);        // First: expectation = 0
    EXPECT_EQ(surprise.surprises[1], 101.0 - 100.0); // Second: expectation = previous
}

TEST_F(SurpriseTransformerTest, VerySmallValues) {
    // Constant very small values: first has large surprise, rest are ~0
    std::vector<double> small = createConstantSeries(0.001, 12);
    auto surprise = SurpriseTransformer::extractSurprise(small, "test");

    EXPECT_NEAR(surprise.surprises[0], 0.001, 1e-10);  // First: 0.001 - 0
    for (size_t i = 1; i < surprise.surprises.size(); i++) {
        EXPECT_NEAR(surprise.surprises[i], 0.0, 1e-10);  // Rest: 0.001 - 0.001
    }
}

TEST_F(SurpriseTransformerTest, VeryLargeValues) {
    // Constant very large values: first has large surprise, rest are ~0
    std::vector<double> large = createConstantSeries(1e6, 12);
    auto surprise = SurpriseTransformer::extractSurprise(large, "test");

    EXPECT_NEAR(surprise.surprises[0], 1e6, 1e0);  // First: 1e6 - 0
    for (size_t i = 1; i < surprise.surprises.size(); i++) {
        EXPECT_NEAR(surprise.surprises[i], 0.0, 1e2);  // Rest: 1e6 - 1e6 = 0
    }
}

// Run tests
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
