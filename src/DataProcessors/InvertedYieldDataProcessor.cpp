//
//  InvertedYieldDataProcessor.cpp
//  InvertedYieldCurveTrader
//
//  Created by Ryan Hamby on 10/1/23.
//
#include "InvertedYieldDataProcessor.hpp"
#include "InvertedYieldStatsCalculator.hpp"
#include "S3ObjectRetriever.hpp"
#include "../Lambda/AlphaVantageDataRetriever.hpp"
#include "StatsCalculator.hpp"
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
using json = nlohmann::json;

std::vector<double> InvertedYieldDataProcessor::process() {
    const std::string jsonFilePath = "../Lambda/AlphaVantageConstants.json";

    std::ifstream jsonFile(jsonFilePath);
    if (!jsonFile) {
        std::cerr << "Error opening JSON file" << std::endl;
    }

    nlohmann::json jsonData;
    jsonFile >> jsonData;

    std::string jsonString10Year = jsonData.dump();
    std::string jsonString2Year = jsonData.dump();

    std::string objectKeyPrefix10Year = jsonData["yield-10-year"]["s3_object_key_prefix"];
    std::string objectKeyPrefix2Year = jsonData["yield-2-year"]["s3_object_key_prefix"];

    // Temporary hardcode
    // TODO: Configure to be the most recent day once EventBridge gets set up
    std::string objectKey10Year = objectKeyPrefix10Year + "/2023-09-21";
    std::string objectKey2Year = objectKeyPrefix2Year + "/2023-09-21";

    std::string bucketName = "alpha-insights";
    S3ObjectRetriever s3ObjectRetriever10Year(bucketName, objectKey10Year);
    S3ObjectRetriever s3ObjectRetriever2Year(bucketName, objectKey2Year);

    if (s3ObjectRetriever10Year.RetrieveJson(jsonString10Year) &&
        s3ObjectRetriever2Year.RetrieveJson(jsonString2Year)) {

        InvertedYieldStatsCalculator calculator;
        
        calculator.setData(jsonString10Year, jsonString2Year);
        for (auto &datapoint: calculator.getData()) {
            std::cout << datapoint << std::endl;
        }

        // Process the retrieved JSON data to calculate confidence score
        std::tuple<double, double> meanAndStdDev = calculator.calculateMeanAndStdDev();
        
        // TODO: remove confidence score at bottom level, and instead use covariance to calculate it at top level, in relation to inverted yield
//        double confidenceScore = std::get<0>(meanAndStdDev); // Use the mean as the confidence score
//        std::cout << "Confidence Score: " << confidenceScore << std::endl;
    } else {
        std::cerr << "Failed to retrieve JSON data from S3" << std::endl;
    }

    std::vector<double> invertedYieldData;

    return invertedYieldData;
};
