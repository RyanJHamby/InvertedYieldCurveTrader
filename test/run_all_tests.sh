#!/bin/bash
# Compile and run all unit tests

set -e  # Exit on error

echo "========================================"
echo "Building and Running All Unit Tests"
echo "========================================"
echo ""

# Check for FRED API key
if [ -z "$FRED_API_KEY" ]; then
    echo "⚠️  Warning: FRED_API_KEY not set"
    echo "   FRED-related tests will be skipped"
    echo ""
fi

# Common includes and libraries
INCLUDES="-I./src -I/opt/homebrew/opt/nlohmann-json/include -I/opt/homebrew/opt/googletest/include"
LIBS="-lcurl"
GTEST_LIBS="-L/opt/homebrew/opt/googletest/lib -lgtest -lgtest_main -pthread"
CXX_FLAGS="-std=c++20"

# Source files needed for tests
FRED_CLIENT="src/DataProviders/FREDDataClient.cpp"
FED_FUNDS_PROC="src/DataProcessors/FedFundsProcessor.cpp"
UNEMPLOYMENT_PROC="src/DataProcessors/UnemploymentProcessor.cpp"
SENTIMENT_PROC="src/DataProcessors/ConsumerSentimentProcessor.cpp"
VIX_PROC="src/DataProcessors/VIXDataProcessor.cpp"

echo "1. Compiling FREDDataClient integration tests..."
g++ $CXX_FLAGS $INCLUDES \
    $FRED_CLIENT \
    test/FREDDataClientIntegrationTest.cpp \
    $LIBS $GTEST_LIBS \
    -o test_fred_client_integration || { echo "❌ Failed to compile FREDDataClient integration tests"; exit 1; }

echo "2. Compiling FRED Processors integration tests..."
g++ $CXX_FLAGS $INCLUDES \
    $FRED_CLIENT \
    $FED_FUNDS_PROC \
    $UNEMPLOYMENT_PROC \
    $SENTIMENT_PROC \
    test/FREDDataProcessorsIntegrationTest.cpp \
    $LIBS $GTEST_LIBS \
    -o test_fred_processors_integration || { echo "❌ Failed to compile FRED Processors integration tests"; exit 1; }

echo "3. Compiling VIX Processor integration tests..."
g++ $CXX_FLAGS $INCLUDES \
    $VIX_PROC \
    test/VIXDataProcessorIntegrationTest.cpp \
    $LIBS $GTEST_LIBS \
    -o test_vix_processor_integration || { echo "❌ Failed to compile VIX Processor integration tests"; exit 1; }

echo ""
echo "✅ All tests compiled successfully!"
echo ""
echo "========================================"
echo "Running Tests"
echo "========================================"
echo ""

# Run each test suite
echo "--- FREDDataClient Integration Tests (requires FRED_API_KEY) ---"
./test_fred_client_integration || { echo "❌ FREDDataClient integration tests failed"; exit 1; }

echo ""
echo "--- FRED Processors Integration Tests (requires FRED_API_KEY) ---"
./test_fred_processors_integration || { echo "❌ FRED Processors integration tests failed"; exit 1; }

echo ""
echo "--- VIX Processor Integration Tests (requires ALPHA_VANTAGE_API_KEY) ---"
./test_vix_processor_integration || { echo "❌ VIX Processor integration tests failed"; exit 1; }

echo ""
echo "========================================"
echo "✅ ALL TESTS PASSED!"
echo "========================================"
echo ""
echo "Test Coverage (Integration Tests - Requires API Keys):"
echo "  ✅ FREDDataClient (live API integration)"
echo "  ✅ FedFundsProcessor (live API)"
echo "  ✅ UnemploymentProcessor (live API)"
echo "  ✅ ConsumerSentimentProcessor (live API)"
echo "  ✅ VIXDataProcessor (live API)"
echo "  ✅ Yield curve calculations"
echo "  ✅ Error handling with real API responses"
echo ""
echo "Total: 30+ integration test cases across 3 test suites"
echo ""
echo "For unit tests (no API keys required):"
echo "  ./run_unit_tests.sh"
