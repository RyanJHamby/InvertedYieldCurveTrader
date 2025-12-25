//
//  SurpriseTransformer.hpp
//  InvertedYieldCurveTrader
//
//  Converts economic indicator levels to macro surprises (expectations-adjusted innovations).
//  ε_t = X_t - E[X_t]
//
//  Created by Ryan Hamby on 12/25/25.
//

#ifndef SURPRISE_TRANSFORMER_HPP
#define SURPRISE_TRANSFORMER_HPP

#include <vector>
#include <string>
#include <map>
#include <stdexcept>
#include <cmath>

/**
 * IndicatorSurprise: Result of surprise extraction for a single indicator
 */
struct IndicatorSurprise {
    std::string indicator;                   // Indicator name (e.g., "CPI", "NFP")
    std::vector<double> surprises;           // ε_t (should be mean ≈ 0)
    std::vector<double> rawValues;           // X_t (original indicator levels)
    std::vector<double> expectations;        // E[X_t] (what market expected)
    std::string expectationSource;           // "survey", "ar1_forecast", "previous", or "none"
    bool isValidated;                        // true if mean(surprises) ≈ 0
    double meanSurprise;                     // actual mean (for diagnostics)
    std::string validationMessage;           // error details if validation fails
};

/**
 * SurpriseTransformer: Convert indicator levels to macro surprises
 *
 * Surprise = Actual - Expected
 * where Expected is determined by hierarchy:
 * 1. Survey consensus (most accurate, from market participants)
 * 2. AR(1) rolling forecast (self-generating, for missing survey data)
 * 3. Previous release (for sticky/low-frequency data)
 * 4. Zero (fallback)
 */
class SurpriseTransformer {
public:
    /**
     * Extract surprise from indicator levels
     *
     * @param levels: Time series of indicator values (most recent first)
     * @param indicator: Indicator name (e.g., "CPI", "NFP", "GDP")
     * @param lookbackMonths: Window size for AR(1) estimation (default 12)
     * @return IndicatorSurprise struct with surprises and metadata
     */
    static IndicatorSurprise extractSurprise(
        const std::vector<double>& levels,
        const std::string& indicator,
        int lookbackMonths = 12
    );

    /**
     * Validate that surprises have zero mean (indicating rational expectations)
     *
     * @param surprise: IndicatorSurprise struct to validate
     * @param tolerance: Acceptable mean deviation (default 0.05)
     * @return true if |mean(surprises)| < tolerance
     */
    static bool validateZeroMean(
        const IndicatorSurprise& surprise,
        double tolerance = 0.05
    );

    /**
     * Load survey expectations for an indicator from external source
     * (Stub for future integration with consensus data feeds)
     *
     * @param indicator: Indicator name
     * @return Vector of survey expectations (0 if not available)
     */
    static std::vector<double> loadSurveyExpectations(const std::string& indicator);

private:
    /**
     * Forecast next value using AR(1) model
     * X_t = α + β * X_{t-1} + noise
     *
     * Estimated via rolling regression on past lookbackMonths
     *
     * @param history: Historical values (in order)
     * @return Forecast for next value
     */
    static double forecastAR1(const std::vector<double>& history);

    /**
     * Extract AR(1) coefficients from historical data
     *
     * @param history: Historical values
     * @param alpha: Output parameter for intercept
     * @param beta: Output parameter for slope
     */
    static void estimateAR1Coefficients(
        const std::vector<double>& history,
        double& alpha,
        double& beta
    );
};

#endif // SURPRISE_TRANSFORMER_HPP
