//
//  StockDataRetriever.cpp
//  InvertedYieldCurveTrader
//
//  Created by Ryan Hamby on 11/1/23.
//

#include "StockDataRetriever.hpp"
#include "S3ObjectRetriever.hpp"
#include "../Lambda/AlphaVantageDataRetriever.hpp"
#include "StatsCalculator.hpp"
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

std::vector<double> StockDataRetriever::retrieve() {
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
        std::cout << "vals" << std::endl;
        for (auto &i : vals) {
            std::cout << i << std::endl;
        }
        return calculator.getData();
    } else {
        std::cerr << "Failed to retrieve JSON data from S3" << std::endl;
    }

    return stockData;
};
