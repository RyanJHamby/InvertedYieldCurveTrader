# InvertedYieldCurveTrader
Optimizes stock trade profits based on trends in the inverted yield curve

![InvertedYieldCurveTraderDiagram](https://github.com/RyanJHamby/InvertedYieldCurveTrader/blob/main/images/InvertedYieldTrader.drawio.jpg)

# How I worked on this project
- I enjoy reading articles all over the web about tech and finance, and stumbled upon a Forbes article that referenced the [strongest stock market indicators](https://www.forbes.com/uk/advisor/investing/stock-market-indicators-investors-need-to-know/), and multiple view points called out inverted yield as potentially the strongest indicator of a recession. Seeing this article sparked curiousity to create a trader that would take these factors into account and calculate their covariance with the inverted yield curve.
- I started from the general idea of the diagram above to guide through the implementation steps. I designed this project architecture myself, and I made it with the intention of trying to enhance my own techniques of trying to time the market.
- I spent most of my time initially learning how to use CMake, compile libraries like nlohmann, provisioning AWS resources, integrating with the AWS C++ SDK, and testing calls to the Alpha Vantage API.
- I've been actively working on optimizing the speed of the functions and looking for ways to incorporate multithreading into the per-minute trading decisions.
- Ideally, I would be able to run the trading in real-time, but I was not able to find a free tier for this data. Thus, I ran the calculations of confidence score on a daily basis and used [3-length-slope-window analysis](https://github.com/RyanJHamby/InvertedYieldCurveTrader/blob/main/src/DataProcessors/StockDataProcessor.cpp#L70-L102) to provide more accurate per-minute calculations and decision tools.

# How to navigate this project
- The main function which was uploaded to AWS Lambda and runs every morning is the [MultiVarAnalysisWorkflow.cpp](https://github.com/RyanJHamby/InvertedYieldCurveTrader/blob/main/src/MultiVarAnalysisWorkflow.cpp). It calls functions in the [DataProcessors folder](https://github.com/RyanJHamby/InvertedYieldCurveTrader/tree/main/src/DataProcessors) and interact with the [AwsClients](https://github.com/RyanJHamby/InvertedYieldCurveTrader/tree/main/src/AwsClients).
- The AWS Lambda function and constants for the API calls are stored in the [Lambda folder](https://github.com/RyanJHamby/InvertedYieldCurveTrader/tree/main/src/Lambda). The [alpha_vantage_data_retriever.py file](https://github.com/RyanJHamby/InvertedYieldCurveTrader/blob/main/src/Lambda/alpha_vantage_data_retriever.py) writes the relevant AlphaVantage data to S3, which is consumed by the C++ driver.
- The tests folder uses GoogleTest to unit test the functions. See [TEST_GUIDE.md](TEST_GUIDE.md) for running unit tests (no API keys required) vs integration tests (requires API keys).

# Economic Data Sources & Update Frequency

The system fetches 8 economic indicators daily via AWS Lambda triggered by EventBridge:

| Indicator | Source | Data Frequency | Notes |
|-----------|--------|----------------|-------|
| **CPI / Inflation** | FRED (CPIAUCSL) | Monthly | Released ~12 days after month ends |
| **Treasury 10Y Yield** | FRED (DGS10) | Daily | Updated after 4:30 PM ET market close |
| **Treasury 2Y Yield** | FRED (DGS2) | Daily | Updated after 4:30 PM ET market close |
| **Fed Funds Rate** | FRED (FEDFUNDS) | Monthly | Lagged by 1 month |
| **Unemployment Rate** | FRED (UNRATE) | Monthly | Released 1st Friday of month |
| **GDP Growth** | FRED (A191RL1Q225SBEA) | Quarterly | ~1 month after quarter ends |
| **Consumer Sentiment** | FRED (UMCSENT) | Monthly | Preliminary (mid-month) + Final (end-month) |
| **VIX (Volatility Index)** | Alpha Vantage | Daily | Updated after 4:15 PM ET market close |

**Fetch Strategy**: Lambda function runs once daily (early morning ET) and fetches all available indicators. FRED has unlimited free API requests, Alpha Vantage has 25 requests/day on free tier. Some indicators (monthly) won't have new data every day, but fetching daily ensures fresh data is captured immediately upon release.

**API Keys Required**:
- FRED: Free, unlimited. Get from https://fred.stlouisfed.org/docs/api/api_key.html
- Alpha Vantage: Free tier has 25 requests/day. Get from https://www.alphavantage.co/support/#api-key

# Multivariate Covariance Analysis (Step 1.3)

The system now implements full 8x8 multivariate covariance analysis using the Eigen library:

**Components**:
- **DataAligner**: Aligns indicators with different frequencies (daily, monthly, quarterly) to a common monthly timeline
  - Daily data (VIX, Treasury yields): Downsamples to monthly (every ~21 trading days)
  - Quarterly data (GDP): Linear interpolation to monthly
  - Monthly data (CPI, unemployment, etc.): Used as-is

- **CovarianceMatrix**: 8x8 covariance matrix with metadata
  - Supports querying covariances by indicator name
  - Calculates Frobenius norm (overall economic regime volatility)
  - Includes validation (symmetry, non-negative variance)

- **CovarianceCalculator**: Extended to support both 2x2 (backward compatible) and 8x8 matrices
  - Full formula: `Cov(X,Y) = Σ[(xi - μx)(yi - μy)] / (n-1)` (unbiased estimator)
  - Validates input data alignment and consistency

**Signal Generation**:
- Uses Frobenius norm of covariance matrix as trading signal strength
- Formula: `||Cov||_F = sqrt(Σ Σ cov(i,j)²)`
- Represents overall economic regime volatility

**Testing**:
- 40+ unit tests for DataAligner and CovarianceCalculator
- All tests use mock data (no API keys required)
- Validates data alignment, matrix symmetry, variance properties

# Why I built the project this way
- Initially I did not know about the rate limiting from the AlphaVantage API, so I was under the impression that I could stack on market predictors (GDP, inverted yield, inflation, interest rate, etc) on the order of O(n). Unfortunately, I could only use one without being throttled, so I chose to use inflation as the primary indicator and use that in conjunction with inverted yield.
- It was simplest to write infrastructure-provisioning code in Python, so I wrote it as a Python Lambda locally and uploaded it to Lambda, then set up an EventBridge to update the data daily.
- For Step 1.3, I used Eigen for efficient 8x8 matrix operations and implemented data alignment to handle mixed-frequency economic data (daily yields, monthly inflation, quarterly GDP).

# If I had more time I would do this
- Incorporate Boost library to unlock the use of more functionality, use smart pointers to help prevent memory leaks, and allow adoption of concurrency with thread creation, synchronization primitives, and other utilities.
- Add more unit tests and start writing integration tests for the system.
- Avoid storing unnecessary data. Since the API calls return either the most 100 recent values or the past month (which could be 50,000+), this led to very large vectors that needed to be stored at least temporarily and wasted space.
- Run a profiler on the code to understand the bottlenecks and optimize speed in the code.
