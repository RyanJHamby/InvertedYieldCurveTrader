//
//  StockDataProcessor.hpp
//  InvertedYieldCurveTrader
//
//  Created by Ryan Hamby on 11/1/23.
//

#ifndef StockDataProcessor_hpp
#define StockDataProcessor_hpp

#include <stdio.h>
#include <vector>

class StockDataProcessor {
public:
    std::vector<double> retrieve();
    double calculateWindowSlope(const std::vector<double>& data, int start, int windowSize);
    std::vector<double> analyzeStockData(const std::vector<double>& stockData);
};

#endif /* StockDataRetriever_hpp */
