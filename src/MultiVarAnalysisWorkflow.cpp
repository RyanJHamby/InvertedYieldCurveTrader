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
#include "DataProcessors/DataAligner.hpp"
#include "DataProcessors/FedFundsProcessor.hpp"
#include "DataProcessors/UnemploymentProcessor.hpp"
#include "DataProcessors/ConsumerSentimentProcessor.hpp"
#include "DataProcessors/VIXDataProcessor.hpp"
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
            // Calculate 8x8 multivariate covariance matrix for all economic indicators

            try {
                // Fetch all 8 economic indicators
                std::map<std::string, std::vector<double>> rawData;

                // Get API keys from environment (or constants)
                const char* fredApiKey = std::getenv("FRED_API_KEY");
                const char* alphaVantageKey = std::getenv("ALPHA_VANTAGE_API_KEY");

                if (!fredApiKey || !alphaVantageKey) {
                    std::cerr << "Error: FRED_API_KEY and ALPHA_VANTAGE_API_KEY environment variables must be set" << std::endl;
                    return 1;
                }

                std::string fredKeyStr(fredApiKey);
                std::string alphaKeyStr(alphaVantageKey);

                std::cout << "Fetching all 8 economic indicators..." << std::endl;

                // Fetch FRED data
                InflationDataProcessor inflationProcessor;
                FedFundsProcessor fedFundsProcessor;
                UnemploymentProcessor unemploymentProcessor;
                ConsumerSentimentProcessor sentimentProcessor;
                GDPDataProcessor gdpProcessor;
                InterestRateDataProcessor interestRateProcessor;
                InvertedYieldDataProcessor invertedYieldProcessor;
                VIXDataProcessor vixProcessor;

                rawData["inflation"] = inflationProcessor.process(fredKeyStr, 10);
                rawData["fed_funds"] = fedFundsProcessor.process(fredKeyStr, 12);
                rawData["unemployment"] = unemploymentProcessor.process(fredKeyStr, 12);
                rawData["consumer_sentiment"] = sentimentProcessor.process(fredKeyStr, 12);
                rawData["gdp"] = gdpProcessor.process(fredKeyStr, 8);

                // Fetch treasury yields and calculate inverted yield curve
                invertedYieldProcessor.process(fredKeyStr);
                rawData["inverted_yield"] = invertedYieldProcessor.getRecentValues();

                // Fetch VIX from Alpha Vantage
                rawData["vix"] = vixProcessor.process(alphaKeyStr, 30);

                std::cout << "Aligning all indicators to monthly frequency..." << std::endl;

                // Align all indicators to monthly frequency
                auto alignedData = DataAligner::alignAllIndicators(rawData);

                std::cout << "Calculating 8x8 covariance matrix..." << std::endl;

                // Calculate 8x8 covariance matrix
                CovarianceCalculator covCalculator;
                CovarianceMatrix covMatrix = covCalculator.calculateCovarianceMatrix(alignedData);

                // Get signal strength from Frobenius norm
                double signalStrength = covMatrix.getFrobeniusNorm();

                std::cout << "Covariance matrix calculated successfully." << std::endl;
                covMatrix.print();

                // Write signal strength to output file (replaces old average magnitude)
                std::ofstream outputFile("./output.txt");
                if (!outputFile.is_open()) {
                    std::cerr << "Failed to open the file for writing." << std::endl;
                    return 1;
                }

                outputFile << signalStrength << std::endl;
                outputFile.close();

                std::cout << "Signal strength written to output.txt: " << signalStrength << std::endl;

            } catch (const std::exception& e) {
                std::cerr << "Error in covariance calculation: " << e.what() << std::endl;
                return 1;
            }
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
                                         "trader_cash_value");

            double ongoingMarketValue = prevDayMarketClosingValue;
            double ongoingTraderPrincipal = prevDayStartingTraderPrincipal;
            double ongoingTraderCashAccount = prevDayStartingTraderCashAccount;

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
            item["market_starting_shares"] = Aws::DynamoDB::Model::AttributeValue().SetN(startingSharesInMarket);
            item["performance_per_minute"] = Aws::DynamoDB::Model::AttributeValue().SetS(jsonStringTradingResultsPerMinuteComparedToMarket);

            dynamoDBClient.putDailyResultItem(item, tableName);
                        
            // set up lambda to run this code with script.
            // may need to write the ouput to DDB between script runs. Account for this latency. Let's try with just local file to prevent latency issues
            // set up eventbridge to run this code daily, already setup eventbridge to run lambda for s3 objects at 12 UTC
            
            // graph in quicksights
            // graph the traderP + traderCash, market Value, and potentially the % increase of each, and the delta % between them
        }
    }

    Aws::ShutdownAPI(options); // Should only be called once.
    return result;
}
