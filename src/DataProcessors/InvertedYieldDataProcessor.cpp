//
//  InvertedYieldDataProcessor.cpp
//  InvertedYieldCurveTrader
//
//  Created by Ryan Hamby on 10/1/23.
//
#include "InvertedYieldDataProcessor.hpp"
#include "InvertedYieldStatsCalculator.hpp"
#include "../AwsClients/S3ObjectRetriever.hpp"
#include "../Lambda/AlphaVantageDataRetriever.hpp"
#include "../Utils/Date.hpp"
#include "StatsCalculator.hpp"
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
using json = nlohmann::json;

void InvertedYieldDataProcessor::process(const std::string& fredApiKey) {
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
    std::string objectKey10Year = objectKeyPrefix10Year + "/" + getDateDaysAgo(1);
    std::string objectKey2Year = objectKeyPrefix2Year + "/" + getDateDaysAgo(1);

    std::string bucketName = "alpha-insights";
    S3ObjectRetriever s3ObjectRetriever10Year(bucketName, objectKey10Year);
    S3ObjectRetriever s3ObjectRetriever2Year(bucketName, objectKey2Year);

    if (s3ObjectRetriever10Year.RetrieveJson(jsonString10Year) &&
        s3ObjectRetriever2Year.RetrieveJson(jsonString2Year)) {

        InvertedYieldStatsCalculator calculator;
        
        calculator.setData(jsonString10Year, jsonString2Year);

        // Process the retrieved JSON data to calculate confidence score
        double mean = calculator.calculateMean(calculator.getData());
        
        setMean(mean);
        setRecentValues(calculator.getData());
    } else {
        std::cerr << "Failed to retrieve JSON data from S3" << std::endl;
    }
};

double InvertedYieldDataProcessor::getMean() {
    return this->mean;
};

std::vector<double> InvertedYieldDataProcessor::getRecentValues() {
    return this->recentValues;
};

void InvertedYieldDataProcessor::setMean(double inputMean) {
    this->mean = inputMean;
};

void InvertedYieldDataProcessor::setRecentValues(std::vector<double> inputRecentValues) {
    this->recentValues = inputRecentValues;
};
