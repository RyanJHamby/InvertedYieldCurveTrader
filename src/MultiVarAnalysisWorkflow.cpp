//
//  MultiVarAnalysisWorkflow.cpp
//  InvertedYieldCurveTrader
//
//  Created by Ryan Hamby on 8/27/23.
//

#include <aws/core/Aws.h>
#include <aws/athena/AthenaClient.h>
#include <nlohmann/json.hpp>
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
#include "AwsClients/DynamoDBClient.hpp"
#include "Utils/Date.hpp"

using json = nlohmann::json;
using namespace Aws;
using namespace Aws::Auth;

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
            
            DynamoDBClient dynamoDBClient(clientConfig);
            
            std::string tableName = "InvertedYieldTrader";
            
            double prevDayMarketClosingValue = dynamoDBClient.getDoubleItem(tableName,
                                             "trader_type_with_date",
                                             "inverted_yield_trader_" + getDateDaysAgo(1),
                                             "market_position_value");
            
            double prevDayStartingTraderPrincipal = dynamoDBClient.getDoubleItem(tableName,
                                         "trader_type_with_date",
                                         "inverted_yield_trader_" + getDateDaysAgo(1),
                                         "trader_position_value");
            double prevDayStartingTraderCashAccount = dynamoDBClient.getDoubleItem(tableName,
                                         "trader_type_with_date",
                                         "inverted_yield_trader_" +  getDateDaysAgo(1),
                                         "trader_cash_account_value");
            
            double ongoingMarketValue = prevDayMarketClosingValue;
            double ongoingTraderPrincipal = prevDayStartingTraderPrincipal;
            double ongoingTraderCashAccount = prevDayStartingTraderCashAccount;
            
            double originalMarketPrincipal = 2000.0;
            
            double maxPercentTraderPrincipalToSellPerTransaction = 0.02;
            double maxPercentTraderCashAccountToBuyPerTransaction = 0.02;
            
            // TODO: calculate shares in market on first day and write to DDB --> manually write to DDB table
            double startingSharesInMarket = dynamoDBClient.getDoubleItem(tableName,
                                       "trader_type_with_date",
                                       "inverted_yield_trader_" +  getDateDaysAgo(1),
                                       "market_starting_shares");
            
            for (int i = 0; i < averageSlidingWindowStockScores.size(); ++i) {
                // combine confidence score found from covariances with per-minute trading stock data
                double predictedMarketDelta = confidenceScore * averageSlidingWindowStockScores[i];
                
                if (predictedMarketDelta > 0) {
                    double amountToSell = ongoingTraderPrincipal * maxPercentTraderPrincipalToSellPerTransaction * predictedMarketDelta;
                    ongoingTraderPrincipal -= amountToSell;
                    ongoingTraderCashAccount += amountToSell;
                }
                else if (predictedMarketDelta < 0) {
                    double amountToBuy = ongoingTraderCashAccount * maxPercentTraderPrincipalToSellPerTransaction * predictedMarketDelta;
                    ongoingTraderPrincipal += amountToBuy;
                    ongoingTraderCashAccount -= amountToBuy;
                }
                double totalPortfolioValueAfterTrade = ongoingTraderPrincipal + ongoingTraderCashAccount;
                actualTradingResults.push_back(totalPortfolioValueAfterTrade);
            }
            
            double dailyProfit = actualTradingResults[actualTradingResults.size() - 1] - stockDataResult[stockDataResult.size() - 1] * startingSharesInMarket;
            
            std::vector<double> tradingResultsPerMinuteComparedToMarket;
            for (int i = 0; i < actualTradingResults.size(); ++i) {
                tradingResultsPerMinuteComparedToMarket.push_back(actualTradingResults[i] - stockDataResult[i] * startingSharesInMarket);
            }
            nlohmann::json jsonTradingResultsPerMinuteComparedToMarket = tradingResultsPerMinuteComparedToMarket;
            std::string jsonStringTradingResultsPerMinuteComparedToMarket = jsonTradingResultsPerMinuteComparedToMarket.dump();
            
            Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> item;

            item["trader_type_with_date"] =  Aws::DynamoDB::Model::AttributeValue().SetS("inverted_yield_trader_" + getDateDaysAgo(0));
            item["covariance_confidence_score"] = Aws::DynamoDB::Model::AttributeValue().SetN(confidenceScore);
            item["market_position_value"] = Aws::DynamoDB::Model::AttributeValue().SetN(ongoingMarketValue);
            item["trader_position_value"] = Aws::DynamoDB::Model::AttributeValue().SetN(ongoingTraderPrincipal);
            item["trader_cash_value"] = Aws::DynamoDB::Model::AttributeValue().SetN(ongoingTraderCashAccount);
            item["daily_profit"] = Aws::DynamoDB::Model::AttributeValue().SetN(dailyProfit);
            item["performance_per_minute"] = Aws::DynamoDB::Model::AttributeValue().SetS(jsonStringTradingResultsPerMinuteComparedToMarket);

            dynamoDBClient.putDailyResultItem(item, tableName);
                        
            // set up lambda to run this code with script.
            // may need to write the ouput to DDB between script runs. Account for this latency. Let's try with just local file to prevent latency issues
            // set up eventbridge to run this code daily
            
            // graph in quicksights
            // graph the traderP + traderCash, market Value, and potentially the % increase of each, and the delta % between them
        }
    }

    Aws::ShutdownAPI(options); // Should only be called once.
    return result;
}
