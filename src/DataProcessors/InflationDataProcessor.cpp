//
//  InflationDataProcessor.cpp
//  InvertedYieldCurveTrader
//
//  Created by Ryan Hamby on 9/21/23.
//

#include "InflationDataProcessor.hpp"
#include "S3ObjectRetriever.hpp"
#include "../Lambda/AlphaVantageDataRetriever.hpp"
#include <include/nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <vector>

std::vector<double> InflationDataProcessor::process() {

    const std::string jsonFilePath = "AlphaVantageConstants.json";

    std::ifstream jsonFile(jsonFilePath);
    if (!jsonFile) {
        std::cerr << "Error opening JSON file" << std::endl;
    }

    nlohmann::json jsonData;
    jsonFile >> jsonData;
    
    std::string objectKey = jsonData["inflation"]["s3_object_key"];
    std::string bucketName = "alpha-insights";
    S3ObjectRetriever s3ObjectRetriever(bucketName, objectKey);

    if (!s3ObjectRetriever.Initialize()) {
        std::cerr << "Failed to initialize S3ObjectRetriever" << std::endl;
    }
    
    std::cout << "here inflation" << std::endl;
    
//    if (s3ObjectRetriever.RetrieveJson(jsonData)) {
//        std::cout << "Retrieved JSON data:\n" << jsonData << std::endl;
//    } else {
//        std::cerr << "Failed to retrieve JSON data from S3" << std::endl;
//    }
}
