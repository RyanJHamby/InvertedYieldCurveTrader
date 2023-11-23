//
//  MultiVarAnalysisWorkflow.cpp
//  InvertedYieldCurveTrader
//
//  Created by Ryan Hamby on 8/27/23.
//

#include <aws/core/Aws.h>
#include <aws/athena/AthenaClient.h>
#include <iostream>
#include <fstream>
#include <aws/core/auth/AWSCredentialsProviderChain.h>
#include <aws/dynamodb/DynamoDBClient.h>
#include <aws/dynamodb/model/PutItemRequest.h>
#include <aws/dynamodb/model/GetItemRequest.h>
#include <aws/dynamodb/model/AttributeValue.h>
#include <aws/athena/model/StartQueryExecutionRequest.h>
#include <aws/athena/model/ResultConfiguration.h>
#include "DataProcessors/InflationDataProcessor.hpp"
#include "DataProcessors/GDPDataProcessor.hpp"
#include "DataProcessors/InterestRateDataProcessor.hpp"
#include "DataProcessors/InvertedYieldDataProcessor.hpp"
#include "DataProcessors/CovarianceCalculator.hpp"
#include "DataProcessors/StatsCalculator.hpp"
#include "DataProcessors/InvertedYieldStatsCalculator.hpp"
#include "DataProcessors/StockDataProcessor.hpp"
using namespace Aws;
using namespace Aws::Auth;

std::string getDateDaysAgo(int backwardsOffset);

int main(int argc, char **argv) {
    
    Aws::SDKOptions options;
    Aws::InitAPI(options); // Should only be called once.
    
    int result = 0;
    
    if (argc > 1) {
        Aws::Client::ClientConfiguration clientConfig;
        clientConfig.region = "us-east-1";
               
        auto provider = Aws::MakeShared<DefaultAWSCredentialsProviderChain>("alloc-tag");
        auto creds = provider->GetAWSCredentials();
        if (creds.IsEmpty()) {
            std::cerr << "Failed authentication" << std::endl;
        }
        
        if (std::strcmp(argv[1], "covariance") == 0) {
            // Will only use inflation data for the time being, due to API throttling
            
            InflationDataProcessor inflationDataProcessor;
            InvertedYieldDataProcessor invertedYieldDataProcessor;
            
            std::vector<double> inflationResult = inflationDataProcessor.process();
            invertedYieldDataProcessor.process();
            std::vector<double> yieldResult = invertedYieldDataProcessor.getRecentValues();
            
            StatsCalculator statsCalculator;
            InvertedYieldStatsCalculator invertedYieldStatsCalculator;
            CovarianceCalculator covarianceCalculator;
            
            double inflationKpiMean = statsCalculator.calculateMean(inflationResult);
            double invertedYieldMean = invertedYieldStatsCalculator.calculateMean(yieldResult);
            double inflationCovariance = covarianceCalculator.calculateCovarianceWithInvertedYield(inflationResult, yieldResult, inflationKpiMean, invertedYieldMean);
            
            double averageMagnitudeOfCovariance = (abs(inflationCovariance));
            
            // Need to write intermediate result to output file to avoid API throttling, then will rerun with different instrumentation after a minute
            std::ofstream outputFile("./output.txt");
            if (!outputFile.is_open()) {
                std::cerr << "Failed to open the file for writing." << std::endl;
                return 1;
            }
            
            outputFile << averageMagnitudeOfCovariance << std::endl;
            outputFile.close();
        } else if (std::strcmp(argv[1], "trade") == 0) {
            StockDataProcessor stockDataProcessor;
            std::vector<double> stockDataResult = stockDataProcessor.retrieve();

            double confidenceScore = 0;
            
            // read the average magnitude of covariance from an input file to start trading day
            std::ifstream inputFile("output.txt");
            if (!inputFile.is_open()) {
                std::cerr << "Failed to open the file." << std::endl;
                return 1; // Exit the program with an error code
            }
            std::string line;
            while (std::getline(inputFile, line)) {
                confidenceScore = std::stod(line);
            }
            inputFile.close();

            std::vector<double> averageSlidingWindowStockScores = stockDataProcessor.analyzeStockData(stockDataResult);
            std::vector<double> actualTradingResults;
            
            // Create a DynamoDB client
            Aws::DynamoDB::DynamoDBClient dynamoClient(clientConfig);

            // Specify the table name
            const char* tableName = "InvertedYieldTrader";

            // Specify the key you want to retrieve
            Aws::DynamoDB::Model::AttributeValue keyAttributeValue;
            keyAttributeValue.SetS("inverted_yield_trader_" + getDateDaysAgo(1)); // Replace with the actual key value

            // Create a GetItemRequest
            Aws::DynamoDB::Model::GetItemRequest getItemRequest;
            getItemRequest.SetTableName(tableName);
//            std::cout << "inverted_yield_trader_" + getDateDaysAgo(1) << std::endl;
            getItemRequest.AddKey("trader_type_with_date", keyAttributeValue); // Replace with your actual key attribute name

            // Perform the GetItem request
            auto getItemOutcome = dynamoClient.GetItem(getItemRequest);

            // Check if the request was successful
            if (getItemOutcome.IsSuccess()) {
                const auto& item = getItemOutcome.GetResult().GetItem();
                if (!item.empty()) {
                    // Retrieve and print the attribute value
                    const auto& attributeValue = item.find("market_position_value")->second;
                    std::cout << "Attribute Value: " << attributeValue.GetN() << std::endl; // Replace with your actual attribute name
                } else {
                    std::cout << "Item not found." << std::endl;
                }
            } else {
                // Print the error message if the request fails
                std::cerr << "Error: " << getItemOutcome.GetError().GetMessage() << std::endl;
            }
            
            double dailyStartingTraderPrincipal = 1000.0;
            double dailyStartingTraderCashAccount = 1000.0;
            
            
            double marketPrincipal = 2000.0;
            
            double maxPercentTraderPrincipalToSellPerTransaction = 0.02;
            double maxPercentTraderCashAccountToBuyPerTransaction = 0.02;
            
            double startingSharesInMarket = stockDataResult.size() > 0 ? 2000.0 / stockDataResult[0] : 0;
            
            for (int i = 0; i < averageSlidingWindowStockScores.size(); ++i) {
                // combine confidence score found from covariances with per-minute trading stock data
                double predictedMarketDelta = confidenceScore * averageSlidingWindowStockScores[i];
                
                if (predictedMarketDelta > 0) {
                    double amountToSell = dailyStartingTraderPrincipal * maxPercentTraderPrincipalToSellPerTransaction * predictedMarketDelta;
                    dailyStartingTraderPrincipal -= amountToSell;
                    dailyStartingTraderCashAccount += amountToSell;
                }
                else if (predictedMarketDelta < 0) {
                    double amountToBuy = dailyStartingTraderCashAccount * maxPercentTraderPrincipalToSellPerTransaction * predictedMarketDelta;
                    dailyStartingTraderPrincipal += amountToBuy;
                    dailyStartingTraderCashAccount -= amountToBuy;
                }
                double totalPortfolioValueAfterTrade = dailyStartingTraderPrincipal + dailyStartingTraderCashAccount;
                actualTradingResults.push_back(totalPortfolioValueAfterTrade);
            }
            
            /// AWS Online setup -->
            // need to concat output from prev day. Maybe write day closing vals to DDB. Also write the covariance val to DDB.

            // Create a PutItemRequest
            Aws::DynamoDB::Model::PutItemRequest putItemRequest;
            putItemRequest.SetTableName(tableName);

            // Create an attribute map for the item you want to put
            Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> item;
            item["trader_type_with_date"] =  Aws::DynamoDB::Model::AttributeValue().SetS("inverted_yield_trader_" + getDateDaysAgo(0));
            item["covariance_confidence_score"] = Aws::DynamoDB::Model::AttributeValue().SetN(confidenceScore);
            item["market_position_value"] = Aws::DynamoDB::Model::AttributeValue().SetN(marketPrincipal);
            item["trader_position_value"] = Aws::DynamoDB::Model::AttributeValue().SetN(dailyStartingTraderPrincipal);
            item["trader_cash_value"] = Aws::DynamoDB::Model::AttributeValue().SetN(dailyStartingTraderCashAccount);
            item["daily_profit"] = Aws::DynamoDB::Model::AttributeValue().SetN(confidenceScore);
            // Add more attributes as needed

            // Iterate through the map and add each attribute to the request
            for (const auto& pair : item) {
                putItemRequest.AddItem(pair.first, pair.second);
            }

            // Put the item into the DynamoDB table
            auto outcome = dynamoClient.PutItem(putItemRequest);

            if (outcome.IsSuccess()) {
                std::cout << "Item successfully added to DynamoDB" << std::endl;
            } else {
                std::cerr << "Failed to put item into DynamoDB: " << outcome.GetError().GetMessage() << std::endl;
            }
            
            // PK: inverted_yield_trader_date
            // Indexes: covariance (just use inflation for now), market data time series of shares * stock value, trader principal, trader cash, daily profit
            // ~500 datapoints per DDB entry
            // Sub out any hardcoding of date, should just overwrite the same key a few times to start
            
            // set up lambda to run this code with script.
            // may need to write the ouput to DDB between script runs. Account for this latency
            // set up eventbridge to run this code daily
            
            // graph in quicksights
            // graph the traderP + traderCash, market Value, and potentially the % increase of each, and the delta % between them
//            for (int i = 0; i < actualTradingResults.size(); ++i) {
//                std::cout << actualTradingResults[i] - stockDataResult[i] * startingSharesInMarket << std::endl;
//            }
        }
    }

    Aws::ShutdownAPI(options); // Should only be called once.
    return result;
}

std::string getDateDaysAgo(int daysAgo = 0) {
    // Get the current system time
    auto now = std::chrono::system_clock::now();

    // Subtract daysAgo from the current time
    auto timeAgo = now - std::chrono::hours(24 * daysAgo);

    // Convert the time to a time_t object
    std::time_t currentTime = std::chrono::system_clock::to_time_t(timeAgo);

    // Convert the time_t object to a struct tm object (for local time)
    std::tm* localTime = std::localtime(&currentTime);

    // Extract year, month, and day from the struct tm
    int year = localTime->tm_year + 1900; // Years since 1900
    int month = localTime->tm_mon + 1;    // Months are zero-based
    int day = localTime->tm_mday;

    return std::to_string(year) + "_" + std::to_string(month) + "_" + std::to_string(day);
}

