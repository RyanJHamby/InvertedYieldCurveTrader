//
//  AlphaVantageDataRetriever.hpp
//  InvertedYieldCurveTrader
//
//  Created by Ryan Hamby on 9/19/23.
//

#ifndef AlphaVantageDataRetriever_hpp
#define AlphaVantageDataRetriever_hpp

#include <stdio.h>
#include <string>

class AlphaVantageDataRetriever {
public:
    AlphaVantageDataRetriever(const std::string& apiKey, int maxResults = 10);

    std::string retrieveStockData(const std::string& symbol);

private:
    std::string apiKey_;
    int maxResults_;
};

#endif /* AlphaVantageDataRetriever_hpp */
