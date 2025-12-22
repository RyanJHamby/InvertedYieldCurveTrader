# FRED API Setup Guide

## What is FRED?

FRED (Federal Reserve Economic Data) is a database of economic data maintained by the Federal Reserve Bank of St. Louis. It provides free access to thousands of economic indicators including:

- Inflation (CPI)
- Treasury yields (10Y, 2Y, etc.)
- Federal Funds Rate
- Unemployment Rate
- GDP Growth
- Consumer Sentiment
- ISM Manufacturing PMI
- And many more...

**Best part:** FRED API is completely **free** with **no rate limits** for standard usage.

## Getting Your Free API Key

1. Visit https://fred.stlouisfed.org/
2. Click "My Account" (top right)
3. Create a free account if you don't have one
4. Once logged in, go to https://fred.stlouisfed.org/docs/api/api_key.html
5. Click "Request API Key"
6. Fill out the simple form (name, email, description of use)
7. Your API key will be generated instantly

## Setting Up the API Key

### Option 1: Environment Variable (Recommended)

```bash
# Add to your ~/.zshrc or ~/.bash_profile
export FRED_API_KEY="your_api_key_here"

# Reload your shell
source ~/.zshrc
```

### Option 2: Update Configuration File

Edit `config/fred_config.json` and replace `YOUR_FRED_API_KEY_HERE` with your actual API key:

```json
{
  "fred_api": {
    "api_key": "your_actual_api_key_here",
    ...
  }
}
```

## Testing the Integration

### Compile and Run Test Program

```bash
cd /Users/ryanhamby/Documents/InvertedYieldCurveTrader

# Compile the test program
g++ -std=c++20 -I./src -I/opt/homebrew/opt/nlohmann-json/include \
    src/DataProviders/FREDDataClient.cpp test_fred_client.cpp -lcurl -o test_fred

# Run the test (make sure FRED_API_KEY is set)
./test_fred
```

### Expected Output

If everything works correctly, you should see:

```
Initializing FRED API Client...

=== Test 1: Fetching CPI (Inflation) Data ===

Latest CPI Values:
  2025-11-01: 314.540
  2025-10-01: 314.211
  ...

=== Test 2: Fetching Treasury Yields ===

10-Year Treasury Yield:
  2025-12-20: 4.38
  2025-12-19: 4.42
  ...

2-Year Treasury Yield:
  2025-12-20: 4.25
  2025-12-19: 4.28
  ...

=== Test 3: Inverted Yield Curve Analysis ===

Yield Curve Spread (10Y - 2Y):
  2025-12-20: 0.13% (Normal)
  2025-12-19: 0.14% (Normal)
  ...

=== Test 4: Other Economic Indicators ===

Federal Funds Rate:
  2025-12-01: 4.58
  ...

=== All Tests Passed! ===
FRED API Client is working correctly.
```

## Available FRED Series

Here are the FRED series IDs used in this project:

| Indicator | FRED Series ID | Update Frequency |
|-----------|----------------|------------------|
| Inflation (CPI) | CPIAUCSL | Monthly |
| 10-Year Treasury Yield | DGS10 | Daily |
| 2-Year Treasury Yield | DGS2 | Daily |
| Federal Funds Rate | FEDFUNDS | Monthly |
| Unemployment Rate | UNRATE | Monthly |
| GDP Growth Rate | A191RL1Q225SBEA | Quarterly |
| Consumer Sentiment | UMCSENT | Monthly |
| ISM Manufacturing PMI | NAPM | Monthly |

## Usage in C++ Code

```cpp
#include "DataProviders/FREDDataClient.hpp"

// Initialize client with API key
FREDDataClient client("your_api_key");

// Fetch latest inflation data
auto inflation = client.fetchInflation(10);  // Last 10 values
for (const auto& obs : inflation) {
    std::cout << obs.date << ": " << obs.value << "\n";
}

// Calculate inverted yield curve
auto yieldCurve = client.calculateInvertedYieldCurve(10);
for (const auto& obs : yieldCurve) {
    if (obs.value < 0) {
        std::cout << obs.date << ": INVERTED! (" << obs.value << "%)\n";
    }
}
```

## Troubleshooting

### Error: "FRED API request failed"

- Check your internet connection
- Verify your API key is correct
- Make sure you're not behind a proxy that blocks FRED API

### Error: "Failed to parse FRED API response"

- The FRED API might be temporarily down (rare)
- Check https://fred.stlouisfed.org/ to see if the site is accessible

### Error: "Invalid FRED API response: missing 'observations' key"

- Your API key might be invalid
- The series ID might be wrong
- Try fetching data directly from https://fred.stlouisfed.org/ to verify

## Rate Limits

FRED API has **no rate limits** for standard usage. You can make as many requests as you need for:
- Research purposes
- Personal projects
- Commercial applications

This makes FRED ideal for:
- Daily data collection
- Backtesting (historical data)
- Real-time trading signals

## Additional Resources

- FRED API Documentation: https://fred.stlouisfed.org/docs/api/fred/
- Browse all FRED series: https://fred.stlouisfed.org/tags/series
- FRED Economic Research: https://research.stlouisfed.org/
