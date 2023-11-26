//
//  interestRateDataProcessor.cpp
//  InvertedYieldCurveTrader
//
//  Created by Ryan Hamby on 10/30/23.
//

#include "InterestRateDataProcessor.hpp"
#include "../AwsClients/S3ObjectRetriever.hpp"
#include "../Lambda/AlphaVantageDataRetriever.hpp"
#include "StatsCalculator.hpp"
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

std::vector<double> InterestRateDataProcessor::process() {
    const std::string jsonFilePath = "../Lambda/AlphaVantageConstants.json";

    std::ifstream jsonFile(jsonFilePath);
    if (!jsonFile) {
        std::cerr << "Error opening JSON file" << std::endl;
    }

    nlohmann::json jsonData;
    jsonFile >> jsonData;

    std::string jsonString = jsonData.dump();

    std::string objectKeyPrefix = jsonData["interest_rate"]["s3_object_key_prefix"];

    // Temporary hardcode
    // TODO: Configure to be the most recent day once EventBridge gets set up
    std::string objectKey = objectKeyPrefix + "/2023-09-21";

    std::string bucketName = "alpha-insights";
    S3ObjectRetriever s3ObjectRetriever(bucketName, objectKey);

    std::vector<double> interestRateData;
    
    if (s3ObjectRetriever.RetrieveJson(jsonString)) {
        // Process the retrieved JSON data to calculate confidence score
        StatsCalculator calculator;

        std::cout << jsonString << std::endl;

        calculator.setData(jsonString);

        std::vector<double> InterestRateValues = calculator.getData();
        interestRateData = std::vector<double>(InterestRateValues.begin(), InterestRateValues.begin() + 10);
    } else {
        std::cerr << "Failed to retrieve JSON data from S3" << std::endl;
    }

    return interestRateData;
};
