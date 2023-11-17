//
//  StockDataProcessor.cpp
//  InvertedYieldCurveTrader
//
//  Created by Ryan Hamby on 11/1/23.
//

#include "StockDataProcessor.hpp"
#include "S3ObjectRetriever.hpp"
#include "../Lambda/AlphaVantageDataRetriever.hpp"
#include "StatsCalculator.hpp"
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

std::vector<double> StockDataProcessor::retrieve() {
    const std::string jsonFilePath = "../Lambda/AlphaVantageConstants.json";

    std::ifstream jsonFile(jsonFilePath);
    if (!jsonFile) {
        std::cerr << "Error opening JSON file" << std::endl;
    }

    nlohmann::json jsonData;
    jsonFile >> jsonData;

    std::string jsonString = jsonData.dump();

    std::string objectKeyPrefix = jsonData["time_series"]["s3_object_key_prefix"];

    // Temporary hardcode
    // TODO: Configure to be the most recent day once EventBridge gets set up
    std::string objectKey = objectKeyPrefix + "/2023-09-21";

    std::string bucketName = "alpha-insights";
    S3ObjectRetriever s3ObjectRetriever(bucketName, objectKey);

    std::vector<double> stockData;
    
    if (s3ObjectRetriever.RetrieveJson(jsonString)) {
        // Process the retrieved JSON data to calculate confidence score
        StatsCalculator calculator;

        calculator.setStockData(jsonString);
        std::vector<double> vals = calculator.getData();
        return calculator.getData();
    } else {
        std::cerr << "Failed to retrieve JSON data from S3" << std::endl;
    }

    return stockData;
};

double StockDataProcessor::calculateWindowSlope(const std::vector<double>& data, int start, int windowSize) {
    if (start < 0) {
        return 0.0;
    }
    if (windowSize < 2) {
        return 0.0; // Avoid division by zero
    }
    double initialPrice = data[start];
    double finalPrice = data[start + windowSize - 1];
    return (finalPrice - initialPrice) / windowSize;
};

std::vector<double> StockDataProcessor::analyzeStockData(const std::vector<double>& stockData) {
    std::vector<double> sellConfidences;
    int windowSize1 = 5;
    int windowSize2 = 15;
    int windowSize3 = 60;
    
    for (int i = 0; i < stockData.size(); ++i) {
        // Calculate the slopes for the past 5, 15, and 60 minutes
        double windowSlope1 = calculateWindowSlope(stockData, i - windowSize1, windowSize1);
        double windowSlope2 = calculateWindowSlope(stockData, i - windowSize2, windowSize2);
        double windowSlope3 = calculateWindowSlope(stockData, i - windowSize3, windowSize3);

        // Calculate the sell percentage based on the slopes of the three windows
        double sellPercentage = 0.0;
        if (i <= windowSize1) {
            sellPercentage = windowSlope1;
        }
        else if (i <= windowSize2) {
            sellPercentage = (windowSize1 + windowSize2) / 2.0;
        }
        else {
            sellPercentage = (windowSlope1 + windowSlope2 + windowSlope3) / 3.0;
        }

        // Calculate the sell confidence (can be based on any criteria)
        double currentStockPrice = stockData[i];
        double sellConfidence = sellPercentage * currentStockPrice;

        sellConfidences.push_back(sellConfidence);
    }

    return sellConfidences;
};
