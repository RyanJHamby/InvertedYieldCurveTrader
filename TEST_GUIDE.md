# Test Guide

This project has two types of tests: **unit tests** and **integration tests**.

## Unit Tests (No API Keys Required)

Unit tests verify code logic using mock data. They **don't need any API keys**.

**Files:**
- `test/FREDDataClientUnitTest.cpp` - FRED API parsing and data structures
- `test/VIXDataProcessorUnitTest.cpp` - VIX data parsing and sorting
- `test/DataAlignerUnitTest.cpp` - Frequency alignment (daily/monthly/quarterly)
- `test/CovarianceCalculatorUnitTest.cpp` - 8x8 covariance matrix calculations

**Run:**
```bash
./run_unit_tests.sh
```

**Coverage:**
- 20 FRED tests: JSON parsing, data validation, yield curve calculations
- 17 VIX tests: JSON parsing, data sorting, value ranges, error handling
- 13 DataAligner tests: Downsampling, interpolation, full alignment
- 30+ CovarianceCalculator tests: Symmetry, diagonal, Frobenius norm, backward compatibility
- **Total: 80+ unit tests**
- Runtime: < 1 second

## Integration Tests (Requires API Keys)

Integration tests verify that the code actually works with real APIs. They **require** valid API keys.

**Files:**
- `test/FREDDataClientIntegrationTest.cpp` - FRED API live calls
- `test/FREDDataProcessorsIntegrationTest.cpp` - FRED processors with real data
- `test/VIXDataProcessorIntegrationTest.cpp` - VIX with live data

**Setup:**
```bash
# Get free API keys (takes 5 minutes)
# FRED: https://fred.stlouisfed.org/docs/api/api_key.html (unlimited)
# Alpha Vantage: https://www.alphavantage.co/support/#api-key (25/day free)

export FRED_API_KEY="your_key"
export ALPHA_VANTAGE_API_KEY="your_key"
```

**Run:**
```bash
./run_all_tests.sh
```

**Coverage:**
- 16 FRED integration tests: API calls, error handling, rate limiting
- 11 FRED processor tests: Data extraction and aggregation
- 4 VIX integration tests: Alpha Vantage API integration
- **Total: 31 integration tests**
- Runtime: 30-60 seconds (API calls)

## Quick Start

**Just want to verify the code compiles and parses data correctly?**
```bash
./run_unit_tests.sh
```

**Want to verify it works with real APIs?**
```bash
export FRED_API_KEY="your_key"
export ALPHA_VANTAGE_API_KEY="your_key"
./run_all_tests.sh
```

## Test Structure

```
test/
├── MockData.hpp                           # Mock JSON responses
├── FREDDataClientUnitTest.cpp             # Unit tests (no API)
├── VIXDataProcessorUnitTest.cpp           # Unit tests (no API)
├── FREDDataClientIntegrationTest.cpp      # Integration tests (API)
├── FREDDataProcessorsIntegrationTest.cpp  # Integration tests (API)
└── VIXDataProcessorIntegrationTest.cpp    # Integration tests (API)
```
