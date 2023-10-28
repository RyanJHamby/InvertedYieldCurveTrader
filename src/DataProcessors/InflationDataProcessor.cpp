//
//  InflationDataProcessor.cpp
//  InvertedYieldCurveTrader
//
//  Created by Ryan Hamby on 9/21/23.
//
#include "InflationDataProcessor.hpp"
#include "S3ObjectRetriever.hpp"
#include "../Lambda/AlphaVantageDataRetriever.hpp"
#include "StatsCalculator.hpp"
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
using json = nlohmann::json;

std::vector<double> InflationDataProcessor::process() {
    const std::string jsonFilePath = "../Lambda/AlphaVantageConstants.json";

    std::ifstream jsonFile(jsonFilePath);
    if (!jsonFile) {
        std::cerr << "Error opening JSON file" << std::endl;
    }

    nlohmann::json jsonData;
    jsonFile >> jsonData;

    std::string jsonString = jsonData.dump();

    std::string objectKeyPrefix = jsonData["inflation"]["s3_object_key_prefix"];

    // Temporary hardcode
    // TODO: Configure to be the most recent day once EventBridge gets set up
    std::string objectKey = objectKeyPrefix + "/2023-09-21";

    std::string bucketName = "alpha-insights";
    S3ObjectRetriever s3ObjectRetriever(bucketName, objectKey);

    std::vector<double> inflationData;

    if (s3ObjectRetriever.RetrieveJson(jsonString)) {
        // Process the retrieved JSON data to calculate confidence score
        StatsCalculator calculator;
        calculator.setData(jsonString);
        std::tuple<double, double> meanAndStdDev = calculator.calculateMeanAndStdDev();
        
        std::vector<double> inflationValues = calculator.getData();
        inflationData = std::vector<double>(inflationValues.begin(), inflationValues.begin() + 10);
        
        for (auto &datapoint: inflationData) {
            std::cout << datapoint << std::endl;
        }
        
        // TODO: remove confidence score at bottom level, and instead use covariance to calculate it at top level, in relation to inverted yield
//        double confidenceScore = std::get<0>(meanAndStdDev); // Use the mean as the confidence score
//        std::cout << "Confidence Score: " << confidenceScore << std::endl;
    } else {
        std::cerr << "Failed to retrieve JSON data from S3" << std::endl;
    }

    return inflationData;
};
