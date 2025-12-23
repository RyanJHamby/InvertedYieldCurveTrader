//
//  DataAlignerUnitTest.cpp
//  InvertedYieldCurveTrader
//
//  Unit tests for DataAligner (no API keys required)
//
//  Created by Ryan Hamby on 12/23/25.
//

#include <gtest/gtest.h>
#include "../src/DataProcessors/DataAligner.hpp"
#include <vector>
#include <cmath>

class DataAlignerUnitTest : public ::testing::Test {
protected:
    // Helper function to create synthetic data
    static std::vector<double> createSyntheticDailyData(int numDays) {
        std::vector<double> data;
        for (int i = numDays - 1; i >= 0; i--) {  // Most recent first
            data.push_back(100.0 + i * 0.1);
        }
        return data;
    }

    static std::vector<double> createSyntheticQuarterlyData(int numQuarters) {
        std::vector<double> data;
        for (int i = numQuarters - 1; i >= 0; i--) {  // Most recent first
            data.push_back(1000.0 + i * 100.0);
        }
        return data;
    }

    static std::vector<double> createSyntheticMonthlyData(int numMonths) {
        std::vector<double> data;
        for (int i = numMonths - 1; i >= 0; i--) {  // Most recent first
            data.push_back(50.0 + i * 5.0);
        }
        return data;
    }
};

// ===== Downsample Tests =====

TEST_F(DataAlignerUnitTest, DownsampleDaily_30To12Months) {
    auto dailyData = createSyntheticDailyData(30 * 21);  // 30 months of daily data

    auto monthlyData = DataAligner::downsampleToMonthly(dailyData, 12);

    EXPECT_EQ(monthlyData.size(), 12);
    EXPECT_GT(monthlyData[0], 0.0);  // Should have valid values
}

TEST_F(DataAlignerUnitTest, DownsampleDaily_InsufficientData) {
    auto dailyData = createSyntheticDailyData(10);  // Only 10 days

    auto monthlyData = DataAligner::downsampleToMonthly(dailyData, 12);

    // Should return fewer months since we don't have enough data
    EXPECT_LT(monthlyData.size(), 12);
    EXPECT_GT(monthlyData.size(), 0);
}

TEST_F(DataAlignerUnitTest, DownsampleDaily_EmptyThrows) {
    std::vector<double> emptyData;

    EXPECT_THROW({
        DataAligner::downsampleToMonthly(emptyData, 12);
    }, std::invalid_argument);
}

TEST_F(DataAlignerUnitTest, DownsampleDaily_ZeroMonthsThrows) {
    auto dailyData = createSyntheticDailyData(30);

    EXPECT_THROW({
        DataAligner::downsampleToMonthly(dailyData, 0);
    }, std::invalid_argument);
}

TEST_F(DataAlignerUnitTest, DownsampleDaily_PreservesMonotone) {
    std::vector<double> dailyData;
    for (int i = 30 * 21 - 1; i >= 0; i--) {
        dailyData.push_back(static_cast<double>(i));  // Monotonically increasing from back
    }

    auto monthlyData = DataAligner::downsampleToMonthly(dailyData, 12);

    // Every downsampled value should be monotonically changing
    for (size_t i = 1; i < monthlyData.size(); i++) {
        EXPECT_TRUE(monthlyData[i] < monthlyData[i-1]);  // Should be decreasing
    }
}

// ===== Interpolate Tests =====

TEST_F(DataAlignerUnitTest, InterpolateQuarterly_4To12Months) {
    auto quarterlyData = createSyntheticQuarterlyData(4);  // 4 quarters

    auto monthlyData = DataAligner::interpolateQuarterlyToMonthly(quarterlyData, 12);

    EXPECT_EQ(monthlyData.size(), 12);
    // All values should be valid
    for (const auto& val : monthlyData) {
        EXPECT_GT(val, 0.0);
        EXPECT_FALSE(std::isnan(val));
    }
}

TEST_F(DataAlignerUnitTest, InterpolateQuarterly_InterpolatedValuesInRange) {
    // Create simple quarterly data: 100, 200, 300, 400 (most recent first)
    std::vector<double> quarterlyData = {400.0, 300.0, 200.0, 100.0};

    auto monthlyData = DataAligner::interpolateQuarterlyToMonthly(quarterlyData, 12);

    // Each interpolated value should be between its surrounding quarters
    for (const auto& val : monthlyData) {
        EXPECT_GE(val, 100.0);  // Min quarterly value
        EXPECT_LE(val, 400.0);  // Max quarterly value
    }
}

TEST_F(DataAlignerUnitTest, InterpolateQuarterly_SingleQuarterThrows) {
    std::vector<double> singleQuarter = {100.0};

    EXPECT_THROW({
        DataAligner::interpolateQuarterlyToMonthly(singleQuarter, 12);
    }, std::invalid_argument);
}

TEST_F(DataAlignerUnitTest, InterpolateQuarterly_EmptyThrows) {
    std::vector<double> emptyData;

    EXPECT_THROW({
        DataAligner::interpolateQuarterlyToMonthly(emptyData, 12);
    }, std::invalid_argument);
}

TEST_F(DataAlignerUnitTest, InterpolateQuarterly_ZeroMonthsThrows) {
    auto quarterlyData = createSyntheticQuarterlyData(4);

    EXPECT_THROW({
        DataAligner::interpolateQuarterlyToMonthly(quarterlyData, 0);
    }, std::invalid_argument);
}

// ===== Align All Indicators Tests =====

TEST_F(DataAlignerUnitTest, AlignAllIndicators_AllPresent) {
    std::map<std::string, std::vector<double>> rawData;

    // Add all 8 indicators
    rawData["vix"] = createSyntheticDailyData(30 * 21);
    rawData["treasury_10y"] = createSyntheticDailyData(30 * 21);
    rawData["treasury_2y"] = createSyntheticDailyData(30 * 21);
    rawData["inverted_yield"] = createSyntheticDailyData(30 * 21);
    rawData["inflation"] = createSyntheticMonthlyData(12);
    rawData["fed_funds"] = createSyntheticMonthlyData(12);
    rawData["unemployment"] = createSyntheticMonthlyData(12);
    rawData["consumer_sentiment"] = createSyntheticMonthlyData(12);
    rawData["gdp"] = createSyntheticQuarterlyData(4);

    auto alignedData = DataAligner::alignAllIndicators(rawData);

    // Should have all 8 indicators + we might get some with 12 values
    EXPECT_GE(alignedData.size(), 8);

    // Check that critical indicators are present
    EXPECT_TRUE(alignedData.count("vix") > 0);
    EXPECT_TRUE(alignedData.count("inflation") > 0);
    EXPECT_TRUE(alignedData.count("gdp") > 0);
}

TEST_F(DataAlignerUnitTest, AlignAllIndicators_EmptyThrows) {
    std::map<std::string, std::vector<double>> emptyMap;

    EXPECT_THROW({
        DataAligner::alignAllIndicators(emptyMap);
    }, std::invalid_argument);
}

TEST_F(DataAlignerUnitTest, AlignAllIndicators_DailyDownsamplesCorrectly) {
    std::map<std::string, std::vector<double>> rawData;
    rawData["vix"] = createSyntheticDailyData(30 * 21);

    auto alignedData = DataAligner::alignAllIndicators(rawData);

    // VIX should have 12 monthly values
    EXPECT_TRUE(alignedData.count("vix") > 0);
    EXPECT_LE(alignedData["vix"].size(), 12);
}

TEST_F(DataAlignerUnitTest, AlignAllIndicators_QuarterlyInterpolatesCorrectly) {
    std::map<std::string, std::vector<double>> rawData;
    rawData["gdp"] = createSyntheticQuarterlyData(4);

    auto alignedData = DataAligner::alignAllIndicators(rawData);

    // GDP should have 12 monthly values after interpolation
    EXPECT_TRUE(alignedData.count("gdp") > 0);
    EXPECT_EQ(alignedData["gdp"].size(), 12);
}

TEST_F(DataAlignerUnitTest, AlignAllIndicators_MonthlyUsedAsIs) {
    std::map<std::string, std::vector<double>> rawData;
    rawData["inflation"] = createSyntheticMonthlyData(12);

    auto alignedData = DataAligner::alignAllIndicators(rawData);

    // Inflation should be 12 values as-is
    EXPECT_TRUE(alignedData.count("inflation") > 0);
    EXPECT_EQ(alignedData["inflation"].size(), 12);
}

TEST_F(DataAlignerUnitTest, AlignAllIndicators_MonthlyTrimmed) {
    std::map<std::string, std::vector<double>> rawData;
    rawData["inflation"] = createSyntheticMonthlyData(24);  // More than 12

    auto alignedData = DataAligner::alignAllIndicators(rawData);

    // Should be trimmed to 12
    EXPECT_EQ(alignedData["inflation"].size(), 12);
}

TEST_F(DataAlignerUnitTest, AlignAllIndicators_MixedPresence) {
    std::map<std::string, std::vector<double>> rawData;

    // Only include some indicators
    rawData["vix"] = createSyntheticDailyData(30 * 21);
    rawData["inflation"] = createSyntheticMonthlyData(12);
    rawData["gdp"] = createSyntheticQuarterlyData(4);

    auto alignedData = DataAligner::alignAllIndicators(rawData);

    // Should have the ones we provided
    EXPECT_TRUE(alignedData.count("vix") > 0);
    EXPECT_TRUE(alignedData.count("inflation") > 0);
    EXPECT_TRUE(alignedData.count("gdp") > 0);

    // Should have properly aligned them
    EXPECT_EQ(alignedData["inflation"].size(), 12);
    EXPECT_EQ(alignedData["gdp"].size(), 12);
}

// Run tests
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
