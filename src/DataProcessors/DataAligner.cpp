//
//  DataAligner.cpp
//  InvertedYieldCurveTrader
//
//  Implementation of data frequency alignment logic
//
//  Created by Ryan Hamby on 12/23/25.
//

#include "DataAligner.hpp"
#include <stdexcept>
#include <iostream>

std::vector<double> DataAligner::downsampleToMonthly(
    const std::vector<double>& dailyData,
    int numMonths) {

    if (dailyData.empty()) {
        throw std::invalid_argument("Daily data cannot be empty");
    }

    if (numMonths <= 0) {
        throw std::invalid_argument("Number of months must be positive");
    }

    std::vector<double> monthlyData;

    // Daily data comes most recent first
    // Each month has approximately 21 trading days
    // Take every 21st value to get monthly data

    for (int month = 0; month < numMonths; month++) {
        int index = month * TRADING_DAYS_PER_MONTH;

        // Ensure we don't go out of bounds
        if (index >= static_cast<int>(dailyData.size())) {
            break;  // Not enough data for this many months
        }

        monthlyData.push_back(dailyData[index]);
    }

    // Ensure we return the requested number of months if possible
    if (monthlyData.size() < static_cast<size_t>(numMonths) && monthlyData.size() < dailyData.size()) {
        // Try to pad with available data if we don't have enough
        for (size_t i = monthlyData.size() * TRADING_DAYS_PER_MONTH;
             i < dailyData.size() && monthlyData.size() < static_cast<size_t>(numMonths);
             i += TRADING_DAYS_PER_MONTH) {
            monthlyData.push_back(dailyData[i]);
        }
    }

    return monthlyData;
}

std::vector<double> DataAligner::interpolateQuarterlyToMonthly(
    const std::vector<double>& quarterlyData,
    int numMonths) {

    if (quarterlyData.empty()) {
        throw std::invalid_argument("Quarterly data cannot be empty");
    }

    if (quarterlyData.size() < 2) {
        throw std::invalid_argument("Need at least 2 quarterly values to interpolate");
    }

    if (numMonths <= 0) {
        throw std::invalid_argument("Number of months must be positive");
    }

    std::vector<double> monthlyData;

    // Quarterly data comes most recent first
    // We need to reverse to interpolate from oldest to newest
    std::vector<double> reversed(quarterlyData.rbegin(), quarterlyData.rend());

    // Interpolate: for each pair of quarters, create 3 monthly values
    for (size_t quarter = 0; quarter < reversed.size() - 1; quarter++) {
        double q_current = reversed[quarter];
        double q_next = reversed[quarter + 1];

        // Linear interpolation between quarters
        for (int month = 0; month < 3; month++) {
            double t = static_cast<double>(month) / 3.0;
            double interpolated = q_current + t * (q_next - q_current);
            monthlyData.push_back(interpolated);

            if (static_cast<int>(monthlyData.size()) >= numMonths) {
                break;
            }
        }

        if (static_cast<int>(monthlyData.size()) >= numMonths) {
            break;
        }
    }

    // Add final quarter values if we need more months
    if (static_cast<int>(monthlyData.size()) < numMonths) {
        for (int month = 0; month < 3 && static_cast<int>(monthlyData.size()) < numMonths; month++) {
            monthlyData.push_back(reversed.back());
        }
    }

    // Reverse back to most recent first
    std::reverse(monthlyData.begin(), monthlyData.end());

    // Ensure we have exactly numMonths values
    if (static_cast<int>(monthlyData.size()) > numMonths) {
        monthlyData.resize(numMonths);
    }

    return monthlyData;
}

std::string DataAligner::detectFrequency(size_t dataSize) {
    if (dataSize >= 24 && dataSize <= 35) {
        return "daily";
    } else if (dataSize >= 10 && dataSize <= 15) {
        return "monthly";
    } else if (dataSize >= 6 && dataSize <= 10) {
        return "quarterly";
    } else {
        return "unknown";
    }
}

std::map<std::string, std::vector<double>> DataAligner::alignAllIndicators(
    const std::map<std::string, std::vector<double>>& rawData) {

    if (rawData.empty()) {
        throw std::invalid_argument("Raw data map cannot be empty");
    }

    std::map<std::string, std::vector<double>> alignedData;
    const int TARGET_MONTHS = 12;

    // List of daily indicators that need downsampling
    const std::vector<std::string> DAILY_INDICATORS = {
        "vix",
        "treasury_10y",
        "treasury_2y",
        "inverted_yield"
    };

    // List of quarterly indicators that need interpolation
    const std::vector<std::string> QUARTERLY_INDICATORS = {
        "gdp"
    };

    // List of monthly indicators (no alignment needed)
    const std::vector<std::string> MONTHLY_INDICATORS = {
        "inflation",
        "fed_funds",
        "unemployment",
        "consumer_sentiment"
    };

    // Process daily indicators (downsample)
    for (const auto& indicator : DAILY_INDICATORS) {
        auto it = rawData.find(indicator);
        if (it != rawData.end() && !it->second.empty()) {
            try {
                alignedData[indicator] = downsampleToMonthly(it->second, TARGET_MONTHS);
            } catch (const std::exception& e) {
                std::cerr << "Warning: Failed to align " << indicator << ": " << e.what() << std::endl;
                alignedData[indicator] = it->second;  // Keep original if alignment fails
            }
        }
    }

    // Process quarterly indicators (interpolate)
    for (const auto& indicator : QUARTERLY_INDICATORS) {
        auto it = rawData.find(indicator);
        if (it != rawData.end() && !it->second.empty()) {
            try {
                alignedData[indicator] = interpolateQuarterlyToMonthly(it->second, TARGET_MONTHS);
            } catch (const std::exception& e) {
                std::cerr << "Warning: Failed to align " << indicator << ": " << e.what() << std::endl;
                alignedData[indicator] = it->second;  // Keep original if alignment fails
            }
        }
    }

    // Process monthly indicators (use as-is, but trim to 12 months)
    for (const auto& indicator : MONTHLY_INDICATORS) {
        auto it = rawData.find(indicator);
        if (it != rawData.end() && !it->second.empty()) {
            std::vector<double> monthly = it->second;
            // Trim to 12 months if needed
            if (static_cast<int>(monthly.size()) > TARGET_MONTHS) {
                monthly.resize(TARGET_MONTHS);
            }
            alignedData[indicator] = monthly;
        }
    }

    // Validate: all indicators should have same number of values (12 months)
    for (const auto& [indicator, values] : alignedData) {
        if (static_cast<int>(values.size()) != TARGET_MONTHS) {
            std::cerr << "Warning: " << indicator << " has " << values.size()
                      << " values instead of " << TARGET_MONTHS << std::endl;
        }
    }

    return alignedData;
}
