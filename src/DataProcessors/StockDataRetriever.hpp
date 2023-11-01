//
//  StockDataRetriever.hpp
//  InvertedYieldCurveTrader
//
//  Created by Ryan Hamby on 11/1/23.
//

#ifndef StockDataRetriever_hpp
#define StockDataRetriever_hpp

#include <stdio.h>
#include <vector>

class StockDataRetriever {
public:
    std::vector<double> retrieve();
};

#endif /* StockDataRetriever_hpp */
