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
echo "  ✅ Error handling"
echo ""
echo "Total: 30+ unit test cases"
echo ""
echo "To run integration tests (requires API keys):"
echo "  export FRED_API_KEY=\"your_fred_api_key\""
echo "  export ALPHA_VANTAGE_API_KEY=\"your_alpha_vantage_key\""
echo "  ./run_all_tests.sh"
echo ""
