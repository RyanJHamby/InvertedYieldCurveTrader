//
//  InflationDataProcessor.cpp
//  InvertedYieldCurveTrader
//
//  Created by Ryan Hamby on 9/21/23.
//

#include "InflationDataProcessor.hpp"
#include "S3ObjectRetriever.hpp"
#include "../Lambda/AlphaVantageDataRetriever.hpp"
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <vector>
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
    
    // temporary hardcode
    // TODO: configure to be most recent day once EventBridge gets set up
    std::string objectKey = objectKeyPrefix + "/2023-09-21";
    
    std::string bucketName = "alpha-insights";
    S3ObjectRetriever s3ObjectRetriever(bucketName, objectKey);
    
    if (s3ObjectRetriever.RetrieveJson(jsonString)) {
        std::cout << "Retrieved JSON data:\n" << jsonString << std::endl;
    } else {
        std::cerr << "Failed to retrieve JSON data from S3" << std::endl;
    }
    
    std::vector<double> inflationData;
    
    return inflationData;
}
