//
//  AlphaVantageDataRetrieverTest.cpp
//  InvertedYieldCurveTrader
//
//  Created by Ryan Hamby on 9/20/23.
//

#include <stdio.h>
#include <gtest/gtest.h>
#include "AlphaVantageDataRetriever.hpp"

TEST(AlphaVantageDataRetrieverTest, RetrieveStockData) {
    std::string apiKey = "YOUR_API_KEY";
    
    AlphaVantageDataRetriever dataRetriever(apiKey);

    std::string symbol = "MSFT";

    std::string data = dataRetriever.retrieveStockData(symbol);

    ASSERT_FALSE(data.empty());
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
