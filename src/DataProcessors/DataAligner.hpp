//
//  DataAligner.hpp
//  InvertedYieldCurveTrader
//
//  Aligns economic indicators with different frequencies to a common monthly timeline
//  Handles downsampling (daily → monthly) and interpolation (quarterly → monthly)
//
//  Created by Ryan Hamby on 12/23/25.
//

#ifndef DataAligner_hpp
#define DataAligner_hpp

#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <cmath>

class DataAligner {
public:
    /**
     * Downsample daily data to monthly frequency
     * Takes the last ~21 values (approximately last trading day of each month)
     *
     * @param dailyData Vector of daily values (most recent first)
     * @param numMonths Number of months to return (default: 12)
     * @return Vector of monthly values (most recent first)
     */
    static std::vector<double> downsampleToMonthly(
        const std::vector<double>& dailyData,
        int numMonths = 12
    );

    /**
     * Interpolate quarterly data to monthly frequency
     * Uses linear interpolation between quarters to create monthly estimates
     *
     * Formula: monthly[i] = quarterly[q] + (quarterly[q+1] - quarterly[q]) * (i % 3) / 3.0
     *
     * @param quarterlyData Vector of quarterly values (most recent first)
     * @param numMonths Number of months to return (default: 12)
     * @return Vector of monthly values (most recent first)
     */
    static std::vector<double> interpolateQuarterlyToMonthly(
        const std::vector<double>& quarterlyData,
        int numMonths = 12
    );

    /**
     * Align all 8 economic indicators to a common monthly frequency
     *
     * Indicator frequencies:
     * - DAILY (30 values): VIX, Treasury 10Y, Treasury 2Y, Inverted Yield
     * - MONTHLY (10-12 values): CPI, Fed Funds, Unemployment, Consumer Sentiment
     * - QUARTERLY (8 values): GDP
     *
     * @param rawData Map of indicator names to their time series values
     * @return Map of aligned indicators, all with 12 monthly values
     */
    static std::map<std::string, std::vector<double>> alignAllIndicators(
        const std::map<std::string, std::vector<double>>& rawData
    );

private:
    /**
     * Determine indicator frequency based on data length
     *
     * @param dataSize Size of the data vector
     * @return "daily" (24-35 values), "monthly" (10-15 values), or "quarterly" (6-10 values)
     */
    static std::string detectFrequency(size_t dataSize);

    /**
     * Get the appropriate number of days per month (for downsampling)
     * Assumes roughly 21 trading days per month
     */
    static constexpr int TRADING_DAYS_PER_MONTH = 21;
};

#endif /* DataAligner_hpp */
