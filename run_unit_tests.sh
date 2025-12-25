#!/bin/bash
# Compile and run unit tests (no API keys required)

set -e  # Exit on error

echo "========================================="
echo "Building and Running Unit Tests"
echo "(No API keys required)"
echo "========================================="
echo ""

# Common includes and libraries
INCLUDES="-I./src -I/opt/homebrew/opt/nlohmann-json/include -I/opt/homebrew/opt/googletest/include"
LIBS="-lcurl"
GTEST_LIBS="-L/opt/homebrew/opt/googletest/lib -lgtest -lgtest_main -pthread"
CXX_FLAGS="-std=c++20"

FRED_CLIENT="src/DataProviders/FREDDataClient.cpp"
VIX_PROC="src/DataProcessors/VIXDataProcessor.cpp"
DATA_ALIGNER="src/DataProcessors/DataAligner.cpp"
COVARIANCE_CALC="src/DataProcessors/CovarianceCalculator.cpp"

# Add Eigen include
EIGEN_INCLUDE="-I/opt/homebrew/opt/eigen/include/eigen3"
INCLUDES="$INCLUDES $EIGEN_INCLUDE"

echo "1. Compiling FRED Data Client unit tests..."
g++ $CXX_FLAGS $INCLUDES \
    $FRED_CLIENT \
    test/FREDDataClientUnitTest.cpp \
    $LIBS $GTEST_LIBS \
    -o test_fred_client_unit || { echo "❌ Failed to compile FRED unit tests"; exit 1; }

echo "2. Compiling VIX Data Processor unit tests..."
g++ $CXX_FLAGS $INCLUDES \
    $VIX_PROC \
    test/VIXDataProcessorUnitTest.cpp \
    $LIBS $GTEST_LIBS \
    -o test_vix_processor_unit || { echo "❌ Failed to compile VIX unit tests"; exit 1; }

echo "3. Compiling DataAligner unit tests..."
g++ $CXX_FLAGS $INCLUDES \
    $DATA_ALIGNER \
    test/DataAlignerUnitTest.cpp \
    $LIBS $GTEST_LIBS \
    -o test_data_aligner_unit || { echo "❌ Failed to compile DataAligner unit tests"; exit 1; }

echo "4. Compiling CovarianceCalculator unit tests..."
g++ $CXX_FLAGS $INCLUDES \
    $COVARIANCE_CALC \
    test/CovarianceCalculatorUnitTest.cpp \
    $LIBS $GTEST_LIBS \
    -o test_covariance_calculator_unit || { echo "❌ Failed to compile CovarianceCalculator unit tests"; exit 1; }

echo "5. Compiling SurpriseTransformer unit tests..."
SURPRISE_TRANSFORMER="src/DataProcessors/SurpriseTransformer.cpp"
g++ $CXX_FLAGS $INCLUDES \
    $SURPRISE_TRANSFORMER \
    test/SurpriseTransformerUnitTest.cpp \
    $LIBS $GTEST_LIBS \
    -o test_surprise_transformer_unit || { echo "❌ Failed to compile SurpriseTransformer unit tests"; exit 1; }

echo "6. Compiling MacroFactorModel unit tests..."
MACRO_FACTOR_MODEL="src/DataProcessors/MacroFactorModel.cpp"
g++ $CXX_FLAGS $INCLUDES \
    $MACRO_FACTOR_MODEL \
    $COVARIANCE_CALC \
    test/MacroFactorModelUnitTest.cpp \
    $LIBS $GTEST_LIBS \
    -o test_macro_factor_model_unit || { echo "❌ Failed to compile MacroFactorModel unit tests"; exit 1; }

echo "7. Compiling Phase 1 Integration tests..."
SURPRISE_TRANSFORMER="src/DataProcessors/SurpriseTransformer.cpp"
g++ $CXX_FLAGS $INCLUDES \
    $SURPRISE_TRANSFORMER \
    $MACRO_FACTOR_MODEL \
    $COVARIANCE_CALC \
    test/Phase1IntegrationTest.cpp \
    $LIBS $GTEST_LIBS \
    -o test_phase1_integration || { echo "❌ Failed to compile Phase 1 integration tests"; exit 1; }

echo "8. Compiling PortfolioRiskAnalyzer unit tests..."
PORTFOLIO_RISK_ANALYZER="src/DataProcessors/PortfolioRiskAnalyzer.cpp"
g++ $CXX_FLAGS $INCLUDES \
    $PORTFOLIO_RISK_ANALYZER \
    $MACRO_FACTOR_MODEL \
    $COVARIANCE_CALC \
    test/PortfolioRiskAnalyzerUnitTest.cpp \
    $LIBS $GTEST_LIBS \
    -o test_portfolio_risk_analyzer_unit || { echo "❌ Failed to compile PortfolioRiskAnalyzer unit tests"; exit 1; }

echo ""
echo "✅ All unit tests compiled successfully!"
echo ""
echo "========================================="
echo "Running Unit Tests"
echo "========================================="
echo ""

echo "--- FRED Data Client Unit Tests ---"
./test_fred_client_unit || { echo "❌ FRED unit tests failed"; exit 1; }

echo ""
echo "--- VIX Data Processor Unit Tests ---"
./test_vix_processor_unit || { echo "❌ VIX unit tests failed"; exit 1; }

echo ""
echo "--- DataAligner Unit Tests ---"
./test_data_aligner_unit || { echo "❌ DataAligner unit tests failed"; exit 1; }

echo ""
echo "--- CovarianceCalculator Unit Tests ---"
./test_covariance_calculator_unit || { echo "❌ CovarianceCalculator unit tests failed"; exit 1; }

echo ""
echo "--- SurpriseTransformer Unit Tests ---"
./test_surprise_transformer_unit || { echo "❌ SurpriseTransformer unit tests failed"; exit 1; }

echo ""
echo "--- MacroFactorModel Unit Tests ---"
./test_macro_factor_model_unit || { echo "❌ MacroFactorModel unit tests failed"; exit 1; }

echo ""
echo "--- Phase 1 Integration Tests ---"
./test_phase1_integration || { echo "❌ Phase 1 integration tests failed"; exit 1; }

echo ""
echo "--- PortfolioRiskAnalyzer Unit Tests ---"
./test_portfolio_risk_analyzer_unit || { echo "❌ PortfolioRiskAnalyzer unit tests failed"; exit 1; }

echo ""
echo "========================================="
echo "✅ ALL UNIT TESTS PASSED!"
echo "========================================="
echo ""
echo "Test Coverage:"
echo "  ✅ FRED Data Client (JSON parsing, data structures)"
echo "  ✅ FRED Series IDs configuration"
echo "  ✅ Yield curve calculations"
echo "  ✅ VIX Data Processor (JSON parsing, data structures)"
echo "  ✅ Data sorting and filtering"
echo "  ✅ Value range validation"
echo "  ✅ DataAligner (frequency alignment, downsampling, interpolation)"
echo "  ✅ CovarianceCalculator (8x8 matrix, Frobenius norm, symmetry)"
echo "  ✅ SurpriseTransformer (AR(1) forecasting, zero-mean validation)"
echo "  ✅ MacroFactorModel (PCA decomposition, archetype matching, stability tracking)"
echo "  ✅ Phase 1 Integration (Full pipeline: Levels → Surprises → Factors)"
echo "  ✅ PortfolioRiskAnalyzer (Exact risk attribution RC_k, scenario analysis, drawdown decomposition)"
echo "  ✅ Error handling and edge cases"
echo ""
echo "Total: 155+ unit test cases"
echo ""
echo "To run integration tests (requires API keys):"
echo "  export FRED_API_KEY=\"your_fred_api_key\""
echo "  export ALPHA_VANTAGE_API_KEY=\"your_alpha_vantage_key\""
echo "  ./run_all_tests.sh"
echo ""
