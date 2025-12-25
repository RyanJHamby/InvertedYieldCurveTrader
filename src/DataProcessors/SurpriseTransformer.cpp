//
//  SurpriseTransformer.cpp
//  InvertedYieldCurveTrader
//
//  Implementation of macro surprise extraction
//
//  Created by Ryan Hamby on 12/25/25.
//

#include "SurpriseTransformer.hpp"
#include <algorithm>
#include <numeric>
#include <cmath>
#include <iostream>

IndicatorSurprise SurpriseTransformer::extractSurprise(
    const std::vector<double>& levels,
    const std::string& indicator,
    int lookbackMonths)
{
    if (levels.empty()) {
        throw std::invalid_argument("Levels vector cannot be empty");
    }

    if (lookbackMonths < 1) {
        throw std::invalid_argument("lookbackMonths must be >= 1");
    }

    IndicatorSurprise result;
    result.rawValues = levels;
    result.surprises.resize(levels.size());
    result.expectations.resize(levels.size());
    result.indicator = indicator;

    std::string lastSource = "none";

    for (size_t i = 0; i < levels.size(); i++) {
        double expected = 0.0;
        std::string source = "none";

        // 1. Try survey data first (most accurate market expectations)
        auto surveyData = loadSurveyExpectations(indicator);
        if (i < surveyData.size() && surveyData[i] != 0.0) {
            expected = surveyData[i];
            source = "survey";
        }
        // 2. Fall back to AR(1) forecast (self-generating expectations)
        // Only use if we have enough history
        else if (i >= static_cast<size_t>(lookbackMonths)) {
            std::vector<double> history(levels.begin(), levels.begin() + i);
            expected = forecastAR1(history);
            source = "ar1_forecast";
        }
        // 3. Or use previous release (crude but works for sticky data)
        else if (i > 0) {
            expected = levels[i - 1];
            source = "previous";
        }
        // 4. Default to zero (no expectations on first data point)
        else {
            expected = 0.0;
            source = "none";
        }

        result.expectations[i] = expected;
        result.surprises[i] = levels[i] - expected;
        lastSource = source;  // Track last source used
    }

    // Compute mean surprise
    result.meanSurprise = 0.0;
    for (double s : result.surprises) {
        result.meanSurprise += s;
    }
    result.meanSurprise /= result.surprises.size();

    // Validate zero-mean property
    double tolerance = 0.05;
    result.isValidated = (std::abs(result.meanSurprise) < tolerance);
    result.expectationSource = lastSource;

    if (!result.isValidated) {
        result.validationMessage = "Mean surprise " + std::to_string(result.meanSurprise) +
                                   " exceeds tolerance " + std::to_string(tolerance) +
                                   "; expectations may be biased";
    }

    return result;
}

bool SurpriseTransformer::validateZeroMean(
    const IndicatorSurprise& surprise,
    double tolerance)
{
    return std::abs(surprise.meanSurprise) < tolerance;
}

std::vector<double> SurpriseTransformer::loadSurveyExpectations(
    const std::string& indicator)
{
    // Stub for future integration with consensus data feeds
    // (Bloomberg, Wall Street Journal, FOMC expectations, etc.)
    // For now, return empty vector (fallback to AR(1) or previous)
    return std::vector<double>();
}

double SurpriseTransformer::forecastAR1(const std::vector<double>& history)
{
    if (history.size() < 2) {
        return 0.0;
    }

    double alpha = 0.0, beta = 0.0;
    estimateAR1Coefficients(history, alpha, beta);

    // Forecast: α + β * last_value
    return alpha + beta * history.back();
}

void SurpriseTransformer::estimateAR1Coefficients(
    const std::vector<double>& history,
    double& alpha,
    double& beta)
{
    if (history.size() < 2) {
        alpha = 0.0;
        beta = 0.0;
        return;
    }

    // AR(1): X_t = α + β * X_{t-1} + ε
    // Estimate via linear regression on consecutive pairs

    int n = history.size() - 1;  // Number of pairs
    double sumXY = 0.0, sumX2 = 0.0, sumX = 0.0, sumY = 0.0;

    // First pass: compute sums for means
    for (int i = 0; i < n; i++) {
        sumX += history[i];
        sumY += history[i + 1];
    }

    double meanX = sumX / n;
    double meanY = sumY / n;

    // Second pass: compute covariance and variance
    for (int i = 0; i < n; i++) {
        double x = history[i] - meanX;
        double y = history[i + 1] - meanY;
        sumXY += x * y;
        sumX2 += x * x;
    }

    // Estimate slope (β)
    if (sumX2 > 1e-10) {
        beta = sumXY / sumX2;
    } else {
        beta = 0.0;
    }

    // Estimate intercept (α)
    alpha = meanY - beta * meanX;
}
