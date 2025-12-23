//
//  ConsumerSentimentProcessor.hpp
//  InvertedYieldCurveTrader
//
//  Processes Consumer Sentiment Index data from FRED API
//
//  Created by Ryan Hamby on 12/22/25.
//

#ifndef ConsumerSentimentProcessor_hpp
#define ConsumerSentimentProcessor_hpp

#include <vector>
#include <string>

class ConsumerSentimentProcessor {
public:
    // Fetch and process Consumer Sentiment data
    // Returns vector of recent values (most recent first)
    std::vector<double> process(const std::string& fredApiKey, int numValues = 12);

    // Get the latest single value
    double getLatestValue(const std::string& fredApiKey);
};

#endif /* ConsumerSentimentProcessor_hpp */
