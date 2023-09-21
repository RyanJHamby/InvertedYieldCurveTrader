//
//  InflationDataProcessor.cpp
//  InvertedYieldCurveTrader
//
//  Created by Ryan Hamby on 9/21/23.
//

#include "InflationDataProcessor.hpp"
#include "S3ObjectRetriever.hpp"
#include "
#include <iostream>
#include <vector>

std::vector<double> InflationDataProcessor::process() {
    S3ObjectRetriever jsonRetriever("your-bucket-name", "your-object-key");

    if (!jsonRetriever.Initialize()) {
        std::cerr << "Failed to initialize S3ObjectRetriever" << std::endl;
    }

    std::string jsonData;
    if (jsonRetriever.RetrieveJson(jsonData)) {
        std::cout << "Retrieved JSON data:\n" << jsonData << std::endl;
    } else {
        std::cerr << "Failed to retrieve JSON data from S3" << std::endl;
    }
}
