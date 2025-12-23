//
//  MockData.hpp
//  InvertedYieldCurveTrader
//
//  Mock data and utilities for unit testing without API keys
//
//  Created by Ryan Hamby on 12/23/25.
//

#ifndef MockData_hpp
#define MockData_hpp

#include <string>

namespace MockData {

// Sample FRED JSON responses
const std::string SAMPLE_FRED_RESPONSE = R"(
{
  "realtime_start": "2025-12-23",
  "realtime_end": "2025-12-23",
  "observation_start": "2025-12-20",
  "observation_end": "2025-12-23",
  "units": "Index 2015=100",
  "output_type": 1,
  "file_type": "json",
  "count": 3,
  "observations": [
    {"realtime_start": "2025-12-23", "realtime_end": "2025-12-23", "date": "2025-12-20", "value": "314.5"},
    {"realtime_start": "2025-12-23", "realtime_end": "2025-12-23", "date": "2025-12-21", "value": "314.7"},
    {"realtime_start": "2025-12-23", "realtime_end": "2025-12-23", "date": "2025-12-22", "value": "315.2"}
  ]
}
)";

const std::string SAMPLE_FRED_TREASURY_RESPONSE = R"(
{
  "realtime_start": "2025-12-23",
  "realtime_end": "2025-12-23",
  "observation_start": "2025-12-20",
  "observation_end": "2025-12-23",
  "units": "Percent",
  "output_type": 1,
  "file_type": "json",
  "count": 3,
  "observations": [
    {"realtime_start": "2025-12-23", "realtime_end": "2025-12-23", "date": "2025-12-20", "value": "4.25"},
    {"realtime_start": "2025-12-23", "realtime_end": "2025-12-23", "date": "2025-12-21", "value": "4.30"},
    {"realtime_start": "2025-12-23", "realtime_end": "2025-12-23", "date": "2025-12-22", "value": "4.28"}
  ]
}
)";

const std::string SAMPLE_ALPHA_VANTAGE_RESPONSE =
R"DELIMITER({
  "Meta Data": {
    "1. Information": "Daily Prices",
    "2. Symbol": "VIX",
    "3. Last Refreshed": "2025-12-22",
    "4. Output Size": "Compact"
  },
  "Time Series (Daily)": {
    "2025-12-22": {
      "1. open": "18.50",
      "2. high": "19.25",
      "3. low": "18.20",
      "4. close": "18.95"
    },
    "2025-12-21": {
      "1. open": "18.00",
      "2. high": "18.75",
      "3. low": "17.85",
      "4. close": "18.50"
    },
    "2025-12-20": {
      "1. open": "17.50",
      "2. high": "18.25",
      "3. low": "17.40",
      "4. close": "18.00"
    }
  }
})DELIMITER";

const std::string INVALID_FRED_RESPONSE = R"(
{
  "error_code": 400,
  "error_message": "Bad Request. The series does not exist."
}
)";

const std::string EMPTY_FRED_RESPONSE = R"(
{
  "realtime_start": "2025-12-23",
  "realtime_end": "2025-12-23",
  "observation_start": "2025-12-23",
  "observation_end": "2025-12-23",
  "units": "Index 2015=100",
  "output_type": 1,
  "file_type": "json",
  "count": 0,
  "observations": []
}
)";

} // namespace MockData

#endif /* MockData_hpp */
