//
//  PositionSizerUnitTest.cpp
//  InvertedYieldCurveTrader
//
//  Unit tests for PositionSizer (risk-aware ES position sizing)
//
//  Created by Ryan Hamby on 12/25/25.
//

#include <gtest/gtest.h>
#include "../src/DataProcessors/PositionSizer.hpp"
#include <cmath>

class PositionSizerTest : public ::testing::Test {
protected:
    // Create mock risk decomposition (ES-like)
    static RiskDecomposition createMockRiskDecomp() {
        RiskDecomposition risk;
        risk.numFactors = 3;
        risk.factorLabels = {"Growth", "Inflation", "Volatility"};
        risk.factorSensitivities = {0.6, -0.2, -0.8};  // ES exposures
        risk.factorRiskContributions = {0.25, 0.08, 0.15};
        risk.componentContributions = {0.52, 0.17, 0.31};
        risk.totalRisk = 0.15;  // 15% daily volatility
        risk.totalVariance = 0.0225;
        return risk;
    }

    // Create calm market regime
    static MacroRegime createCalmRegime() {
        MacroRegime regime;
        regime.vixLevel = 14.0;
        regime.moveIndex = 95.0;
        regime.creditSpread = 110.0;
        regime.yieldCurveSlope = 120.0;
        regime.putCallRatio = 0.9;
        regime.riskLabel = "Risk-On";
        regime.inflationLabel = "Growth-Sensitive";
        regime.fragileLabel = "Stable";
        regime.confidence = 0.80;
        regime.volatilityMultiplier = 1.0;  // Baseline
        return regime;
    }

    // Create stressed market regime
    static MacroRegime createStressedRegime() {
        MacroRegime regime;
        regime.vixLevel = 35.0;
        regime.moveIndex = 140.0;
        regime.creditSpread = 200.0;
        regime.yieldCurveSlope = -20.0;
        regime.putCallRatio = 1.2;
        regime.riskLabel = "Risk-Off";
        regime.inflationLabel = "Inflation-Sensitive";
        regime.fragileLabel = "Fragile";
        regime.confidence = 0.85;
        regime.volatilityMultiplier = 2.5;  // Crisis
        return regime;
    }

    // Create basic constraints
    static PositionConstraint createConstraints() {
        PositionConstraint constraints;
        constraints.maxNotional = 10000000.0;  // $10M max
        constraints.maxLeverageMultiple = 2.0; // 2x leverage
        constraints.maxFactorExposure["Growth"] = 1.0;
        constraints.maxFactorExposure["Inflation"] = 0.5;
        constraints.maxFactorExposure["Volatility"] = 1.0;
        constraints.maxDailyLoss = 100000.0;
        constraints.maxDrawdownFromPeak = 500000.0;
        return constraints;
    }
};

// ===== Basic Position Sizing Tests =====

TEST_F(PositionSizerTest, ComputePositionSizeCalm) {
    double baseNotional = 5000000.0;  // $5M
    auto risk = createMockRiskDecomp();
    auto regime = createCalmRegime();
    auto constraints = createConstraints();

    PositionSizing sizing = PositionSizer::computePositionSize(
        baseNotional, risk, regime, constraints);

    // In calm market, should size to base
    EXPECT_GT(sizing.recommendedNotional, baseNotional * 0.8);
    EXPECT_LE(sizing.recommendedNotional, baseNotional);
}

TEST_F(PositionSizerTest, ComputePositionSizeStressed) {
    double baseNotional = 5000000.0;
    auto risk = createMockRiskDecomp();
    auto regime = createStressedRegime();
    auto constraints = createConstraints();

    PositionSizing sizing = PositionSizer::computePositionSize(
        baseNotional, risk, regime, constraints);

    // In stressed market, should reduce size significantly
    EXPECT_LT(sizing.recommendedNotional, baseNotional / 2.0);
    EXPECT_TRUE(sizing.shouldReduceSize);
}

TEST_F(PositionSizerTest, ComputePositionSizeInverseVolatility) {
    auto risk = createMockRiskDecomp();
    auto constraints = createConstraints();

    auto calmRegime = createCalmRegime();
    calmRegime.volatilityMultiplier = 1.0;

    auto stressedRegime = createStressedRegime();
    stressedRegime.volatilityMultiplier = 2.0;

    double baseNotional = 5000000.0;

    PositionSizing calmSizing = PositionSizer::computePositionSize(
        baseNotional, risk, calmRegime, constraints);
    PositionSizing stressSizing = PositionSizer::computePositionSize(
        baseNotional, risk, stressedRegime, constraints);

    // Stressed regime should result in smaller position
    EXPECT_LT(stressSizing.recommendedNotional, calmSizing.recommendedNotional);
}

TEST_F(PositionSizerTest, PositionSizingWithinConstraints) {
    double baseNotional = 5000000.0;
    auto risk = createMockRiskDecomp();
    auto regime = createCalmRegime();
    auto constraints = createConstraints();

    PositionSizing sizing = PositionSizer::computePositionSize(
        baseNotional, risk, regime, constraints);

    // Should respect max notional
    EXPECT_LE(sizing.recommendedNotional, constraints.maxNotional);

    // Should have valid structure
    EXPECT_GT(sizing.recommendedShares, 0.0);
    EXPECT_GT(sizing.expectedDailyVol, 0.0);
}

// ===== Regime Classification Tests =====

TEST_F(PositionSizerTest, ClassifyRegimeCalmMarket) {
    MacroRegime regime = PositionSizer::classifyRegime(
        12.0,    // VIX (low, < 12)
        90.0,    // MOVE
        85.0,    // Spread (tight, < 100)
        150.0,   // Curve
        0.85     // Put/Call
    );

    // Calm market should be classified as Risk-On or Neutral
    EXPECT_TRUE(regime.riskLabel == "Risk-On" || regime.riskLabel == "Neutral");
    EXPECT_EQ(regime.fragileLabel, "Stable");
    EXPECT_NEAR(regime.volatilityMultiplier, 1.0, 0.15);
}

TEST_F(PositionSizerTest, ClassifyRegimeStressedMarket) {
    MacroRegime regime = PositionSizer::classifyRegime(
        40.0,    // VIX (high, > 30)
        150.0,   // MOVE (high)
        260.0,   // Spread (wide, > 250)
        -50.0,   // Curve (inverted)
        1.3      // Put/Call (high)
    );

    // Stressed market with both high VIX and wide spread should be Risk-Off
    EXPECT_EQ(regime.riskLabel, "Risk-Off");
    EXPECT_EQ(regime.fragileLabel, "Fragile");
    EXPECT_GT(regime.volatilityMultiplier, 2.0);
}

TEST_F(PositionSizerTest, ClassifyRegimeInflationSensitive) {
    MacroRegime regime = PositionSizer::classifyRegime(
        15.0,    // VIX
        100.0,   // MOVE
        120.0,   // Spread
        -50.0,   // Curve (flat/inverted = inflation sensitive)
        0.90     // Put/Call
    );

    EXPECT_EQ(regime.inflationLabel, "Inflation-Sensitive");
}

TEST_F(PositionSizerTest, ClassifyRegimeGrowthSensitive) {
    MacroRegime regime = PositionSizer::classifyRegime(
        13.0,    // VIX
        95.0,    // MOVE
        105.0,   // Spread
        150.0,   // Curve (steep = growth sensitive)
        0.85     // Put/Call
    );

    EXPECT_EQ(regime.inflationLabel, "Growth-Sensitive");
}

// ===== Volatility Multiplier Tests =====

TEST_F(PositionSizerTest, VolatilityMultiplierBaselineCalm) {
    auto regime = createCalmRegime();
    double multiplier = PositionSizer::computeVolatilityMultiplier(regime);

    // Calm market should be near 1.0
    EXPECT_NEAR(multiplier, 1.0, 0.2);
}

TEST_F(PositionSizerTest, VolatilityMultiplierCrisis) {
    auto regime = createStressedRegime();
    double multiplier = PositionSizer::computeVolatilityMultiplier(regime);

    // Crisis should be significantly elevated
    EXPECT_GT(multiplier, 2.0);
}

// ===== Hedging Strategy Tests =====

TEST_F(PositionSizerTest, RecommendHedgeCalmMarket) {
    auto risk = createMockRiskDecomp();
    auto regime = createCalmRegime();
    double position = 5000000.0;

    HedgingStrategy hedge = PositionSizer::recommendHedge(position, risk, regime);

    // Calm market should not need hedging
    EXPECT_FALSE(hedge.needsHedge);
    EXPECT_NEAR(hedge.estimatedCost, 0.0, 1.0);
}

TEST_F(PositionSizerTest, RecommendHedgeHighVIX) {
    auto risk = createMockRiskDecomp();
    auto regime = createStressedRegime();
    regime.vixLevel = 45.0;
    double position = 5000000.0;

    HedgingStrategy hedge = PositionSizer::recommendHedge(position, risk, regime);

    // High VIX should recommend hedging
    EXPECT_TRUE(hedge.needsHedge);
    EXPECT_GT(hedge.hedgeRatio, 0.0);
    EXPECT_GT(hedge.estimatedCost, 0.0);
}

// ===== Stress Test Position Tests =====

TEST_F(PositionSizerTest, StressTestPositionNegative) {
    auto risk = createMockRiskDecomp();
    double position = 5000000.0;

    double loss = PositionSizer::stressTestPosition(position, risk);

    // Should show a loss in crisis scenario
    EXPECT_LT(loss, 0.0);
}

TEST_F(PositionSizerTest, StressTestPositionScales) {
    auto risk = createMockRiskDecomp();

    double loss1 = PositionSizer::stressTestPosition(1000000.0, risk);
    double loss2 = PositionSizer::stressTestPosition(5000000.0, risk);

    // Loss should scale linearly with position size
    EXPECT_NEAR(loss2, loss1 * 5.0, std::abs(loss1) * 0.1);
}

// ===== Hysteresis Tests =====

TEST_F(PositionSizerTest, HysteresisReduceImmediately) {
    auto prevRegime = createCalmRegime();
    auto currRegime = createStressedRegime();

    double currentSize = 5000000.0;
    double recommendedSize = 2000000.0;  // Reduce

    double result = PositionSizer::applyHysteresis(
        currentSize, recommendedSize, prevRegime, currRegime, 1);

    // Should reduce immediately
    EXPECT_EQ(result, recommendedSize);
}

TEST_F(PositionSizerTest, HysteresisIncreaseSlowly) {
    auto prevRegime = createCalmRegime();
    auto currRegime = createCalmRegime();

    double currentSize = 2000000.0;
    double recommendedSize = 5000000.0;  // Increase

    double result = PositionSizer::applyHysteresis(
        currentSize, recommendedSize, prevRegime, currRegime, 1);

    // Should increase slowly (max 20% per day)
    EXPECT_LT(result, recommendedSize);
    EXPECT_LE(result, currentSize * 1.2);
}

TEST_F(PositionSizerTest, HysteresisEstablishedRegime) {
    auto regime = createCalmRegime();

    double currentSize = 2000000.0;
    double recommendedSize = 5000000.0;

    double result = PositionSizer::applyHysteresis(
        currentSize, recommendedSize, regime, regime, 10);  // 10 days = established

    // Should follow recommendation in established regime
    EXPECT_NEAR(result, recommendedSize, 1.0);
}

// ===== VaR Tests =====

TEST_F(PositionSizerTest, ComputeDailyVaR) {
    auto risk = createMockRiskDecomp();
    double position = 5000000.0;

    double var95 = PositionSizer::computeDailyVaR(position, risk, 0.95);
    double var99 = PositionSizer::computeDailyVaR(position, risk, 0.99);

    // 99% VaR should be larger than 95% VaR
    EXPECT_GT(var99, var95);

    // Both should be positive
    EXPECT_GT(var95, 0.0);
    EXPECT_GT(var99, 0.0);
}

// ===== Error Handling Tests =====

TEST_F(PositionSizerTest, ComputePositionSizeNegativeBase) {
    auto risk = createMockRiskDecomp();
    auto regime = createCalmRegime();
    auto constraints = createConstraints();

    EXPECT_THROW(
        PositionSizer::computePositionSize(-1000000.0, risk, regime, constraints),
        std::invalid_argument
    );
}

TEST_F(PositionSizerTest, ComputePositionSizeNoFactors) {
    auto risk = createMockRiskDecomp();
    risk.numFactors = 0;
    auto regime = createCalmRegime();
    auto constraints = createConstraints();

    EXPECT_THROW(
        PositionSizer::computePositionSize(5000000.0, risk, regime, constraints),
        std::invalid_argument
    );
}

// ===== Integration Tests =====

TEST_F(PositionSizerTest, FullWorkflowCalmMarket) {
    double baseNotional = 5000000.0;
    auto risk = createMockRiskDecomp();
    auto regime = createCalmRegime();
    auto constraints = createConstraints();

    // Compute position
    PositionSizing sizing = PositionSizer::computePositionSize(
        baseNotional, risk, regime, constraints);

    // Get hedging strategy
    HedgingStrategy hedge = PositionSizer::recommendHedge(
        sizing.recommendedNotional, risk, regime);

    // Stress test
    double maxLoss = PositionSizer::stressTestPosition(
        sizing.recommendedNotional, risk);

    // Verify consistency
    EXPECT_TRUE(sizing.isWithinConstraints);
    EXPECT_FALSE(hedge.needsHedge);
    EXPECT_LT(maxLoss, 0.0);  // Loss in stress scenario
}

TEST_F(PositionSizerTest, FullWorkflowStressedMarket) {
    double baseNotional = 5000000.0;
    auto risk = createMockRiskDecomp();
    auto regime = createStressedRegime();
    auto constraints = createConstraints();

    // Compute position
    PositionSizing sizing = PositionSizer::computePositionSize(
        baseNotional, risk, regime, constraints);

    // Get hedging strategy
    HedgingStrategy hedge = PositionSizer::recommendHedge(
        sizing.recommendedNotional, risk, regime);

    // Should reduce size and recommend hedge
    EXPECT_LT(sizing.recommendedNotional, baseNotional / 2.0);
    EXPECT_TRUE(hedge.needsHedge);
    EXPECT_GT(hedge.hedgeRatio, 0.0);
}

// Run tests
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
