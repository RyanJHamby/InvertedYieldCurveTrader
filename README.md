# InvertedYieldCurveTrader
Optimizes stock trade profits based on trends in the inverted yield curve


# How I worked on this project
- I enjoy reading articles all over the web about tech and finance, and stumbled upon a Forbes article that referenced the [strongest stock market indicators](https://www.forbes.com/uk/advisor/investing/stock-market-indicators-investors-need-to-know/), and multiple view points called out inverted yield as potentially the strongest indicator of a recession. Seeing this article sparked curiousity to create a trader that would take these factors into account and calculate their covariance with the inverted yield curve.
- I started from the general idea of the diagram above to guide through the implementation steps. I designed this project architecture myself, and I made it with the intention of trying to enhance my own techniques of trying to time the market.
- I spent most of my time initially learning how to use CMake, compile libraries like nlohmann, provisioning AWS resources, integrating with the AWS C++ SDK, and testing calls to the Alpha Vantage API.
- I've been actively working on optimizing the speed of the functions and looking for ways to incorporate multithreading into the per-minute trading decisions.
- Ideally, I would be able to run the trading in real-time, but I was not able to find a free tier for this data. Thus, I ran the calculations of confidence score on a daily basis and used 3-length-slope-window analysis to provide more accurate per-minute calculations and decision tools.

# How to navigate this project
- The main function which was uploaded to AWS Lambda and runs every morning is the [MultiVarAnalysisWorkflow.cpp](https://github.com/RyanJHamby/InvertedYieldCurveTrader/blob/main/src/MultiVarAnalysisWorkflow.cpp). It calls functions in the [DataProcessors folder](https://github.com/RyanJHamby/InvertedYieldCurveTrader/tree/main/src/DataProcessors) and interact with the [AwsClients](https://github.com/RyanJHamby/InvertedYieldCurveTrader/tree/main/src/AwsClients).
- The AWS Lambda function and constants for the API calls are stored in the [Lambda folder](https://github.com/RyanJHamby/InvertedYieldCurveTrader/tree/main/src/Lambda). The [alpha_vantage_data_retriever.py file](https://github.com/RyanJHamby/InvertedYieldCurveTrader/blob/main/src/Lambda/alpha_vantage_data_retriever.py) writes the relevant AlphaVantage data to S3, which is consumed by the C++ driver.
- The tests folder is in progress but uses GoogleTest to unit test the functions.

# Why I built the project this way
- Initially I did not know about the rate limiting from the AlphaVantage API, so I was under the impression that I could stack on market predictors (GDP, inverted yield, inflation, interest rate, etc) on the order of O(n). Unfortunately, I could only use one without being throttled, so I chose to use inflation as the primary indicator and use that in conjunction with inverted yield.
- It was simplest to write infrastructure-provisioning code in Python, so I wrote it as a Python Lambda locally and uploaded it to Lambda, then set up an EventBridge to update the data daily.

# If I had more time I would do this
- Incorporate Boost library to unlock the use of more functionality, use smart pointers to help prevent memory leaks, and allow adoption of concurrency with thread creation, synchronization primitives, and other utilities.
- Add more unit tests and start writing integration tests for the system.
- Avoid storing unnecessary data. Since the API calls return either the most 100 recent values or the past month (which could be 50,000+), this led to very large vectors that needed to be stored at least temporarily and wasted space.
- Run a profiler on the code to understand the bottlenecks and optimize speed in the code.

