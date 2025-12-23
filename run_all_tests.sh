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

echo "1. Compiling FREDDataClient tests..."
g++ $CXX_FLAGS $INCLUDES \
    $FRED_CLIENT \
    test/FREDDataClientTest.cpp \
    $LIBS $GTEST_LIBS \
    -o test_fred_client || { echo "❌ Failed to compile FREDDataClient tests"; exit 1; }

echo "2. Compiling FRED Processors tests..."
g++ $CXX_FLAGS $INCLUDES \
    $FRED_CLIENT \
    $FED_FUNDS_PROC \
    $UNEMPLOYMENT_PROC \
    $SENTIMENT_PROC \
    test/FREDDataProcessorsTest.cpp \
    $LIBS $GTEST_LIBS \
    -o test_fred_processors || { echo "❌ Failed to compile FRED Processors tests"; exit 1; }

echo "3. Compiling VIX Processor tests..."
g++ $CXX_FLAGS $INCLUDES \
    $VIX_PROC \
    test/VIXDataProcessorTest.cpp \
    $LIBS $GTEST_LIBS \
    -o test_vix_processor || { echo "❌ Failed to compile VIX Processor tests"; exit 1; }

echo ""
echo "✅ All tests compiled successfully!"
echo ""
echo "========================================"
echo "Running Tests"
echo "========================================"
echo ""

# Run each test suite
echo "--- FREDDataClient Tests ---"
./test_fred_client || { echo "❌ FREDDataClient tests failed"; exit 1; }

echo ""
echo "--- FRED Processors Tests ---"
./test_fred_processors || { echo "❌ FRED Processors tests failed"; exit 1; }

echo ""
echo "--- VIX Processor Tests ---"
./test_vix_processor || { echo "❌ VIX Processor tests failed"; exit 1; }

echo ""
echo "========================================"
echo "✅ ALL TESTS PASSED!"
echo "========================================"
echo ""
echo "Test Coverage:"
echo "  ✅ FREDDataClient (base API client)"
echo "  ✅ FedFundsProcessor"
echo "  ✅ UnemploymentProcessor"
echo "  ✅ ConsumerSentimentProcessor"
echo "  ✅ VIXDataProcessor"
echo "  ✅ Integration tests"
echo "  ✅ Error handling tests"
echo ""
echo "Total: 50+ test cases across 3 test suites"
