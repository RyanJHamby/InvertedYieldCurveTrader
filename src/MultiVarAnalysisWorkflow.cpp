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
#include "DataProcessors/SurpriseTransformer.hpp"
#include "DataProcessors/MacroFactorModel.hpp"
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
            // PHASE 1: Extract macro surprises and decompose into interpretable factors
            // This is the core analytical pipeline:
            // Step 1.2: Convert economic levels → surprises (information shocks)
            // Step 2.1: Decompose surprise covariance into macro factors (Growth, Inflation, Policy, Volatility)

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

                std::cout << "=========================================" << std::endl;
                std::cout << "PHASE 1: Macro Surprise-Driven Analysis" << std::endl;
                std::cout << "=========================================" << std::endl;
                std::cout << std::endl;

                // STEP 1: Fetch all 8 economic indicators
                std::cout << "Step 1: Fetching all 8 economic indicators..." << std::endl;

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

                invertedYieldProcessor.process(fredKeyStr);
                rawData["inverted_yield"] = invertedYieldProcessor.getRecentValues();

                rawData["vix"] = vixProcessor.process(alphaKeyStr, 30);

                std::cout << "✓ Fetched " << rawData.size() << " indicators" << std::endl;
                std::cout << std::endl;

                // STEP 2: Align to monthly frequency
                std::cout << "Step 2: Aligning all indicators to monthly frequency..." << std::endl;

                auto alignedData = DataAligner::alignAllIndicators(rawData);

                std::cout << "✓ Aligned to 12 monthly observations" << std::endl;
                std::cout << std::endl;

                // STEP 3: Extract surprises (Step 1.2: Surprise Extraction)
                std::cout << "Step 3 (1.2): Extracting macro surprises from levels..." << std::endl;
                std::cout << "  ε_t = X_t − E[X_t]  (information shocks)" << std::endl;
                std::cout << std::endl;

                std::map<std::string, IndicatorSurprise> allSurprises;
                std::map<std::string, std::vector<double>> surpriseVectors;

                for (auto& [indicator, levels] : alignedData) {
                    IndicatorSurprise surprise = SurpriseTransformer::extractSurprise(levels, indicator, 6);
                    allSurprises[indicator] = surprise;
                    surpriseVectors[indicator] = surprise.surprises;

                    std::cout << "  " << indicator << ": ";
                    std::cout << "mean=" << surprise.meanSurprise << ", ";
                    std::cout << "source=" << surprise.expectationSource << ", ";
                    std::cout << "validated=" << (surprise.isValidated ? "yes" : "no") << std::endl;
                }
                std::cout << std::endl;

                // STEP 4: Calculate covariance of SURPRISES (not levels)
                std::cout << "Step 4: Computing 8x8 covariance matrix of surprises..." << std::endl;
                std::cout << "  Σ_ε = Cov(ε_1, ..., ε_8)" << std::endl;
                std::cout << std::endl;

                CovarianceCalculator covCalculator;
                CovarianceMatrix surpriseCovMatrix = covCalculator.calculateCovarianceMatrix(surpriseVectors);

                double frobenius = surpriseCovMatrix.getFrobeniusNorm();
                std::cout << "✓ Covariance matrix computed. Frobenius norm (regime volatility): " << frobenius << std::endl;
                std::cout << std::endl;

                // STEP 5: Decompose surprises into macro factors (Step 2.1)
                std::cout << "Step 5 (2.1): Decomposing surprises into 3 macro factors..." << std::endl;
                std::cout << "  ε_t = B f_t + u_t  (PCA decomposition)" << std::endl;
                std::cout << std::endl;

                MacroFactors factors = MacroFactorModel::decomposeSurpriseCovariance(surpriseCovMatrix, 3);

                std::cout << "✓ Factor decomposition complete" << std::endl;
                std::cout << std::endl;

                // Display factor results
                std::cout << "=========================================" << std::endl;
                std::cout << "Macro Factor Results" << std::endl;
                std::cout << "=========================================" << std::endl;
                std::cout << std::endl;

                std::cout << "Variance Explained: " << (factors.cumulativeVarianceExplained * 100.0) << "%" << std::endl;
                std::cout << std::endl;

                for (int k = 0; k < factors.numFactors; k++) {
                    std::cout << "Factor " << (k + 1) << ": " << factors.factorLabels[k] << std::endl;
                    std::cout << "  Variance: " << factors.factorVariances[k] << std::endl;
                    std::cout << "  Confidence: " << (factors.labelConfidences[k] * 100.0) << "%" << std::endl;
                    std::cout << "  Loadings:" << std::endl;

                    for (int i = 0; i < static_cast<int>(factors.indicatorNames.size()); i++) {
                        double loading = factors.loadings(i, k);
                        if (std::abs(loading) > 0.05) {  // Only show meaningful loadings
                            std::cout << "    " << factors.indicatorNames[i] << ": " << loading << std::endl;
                        }
                    }
                    std::cout << std::endl;
                }

                // Write results to file
                std::ofstream outputFile("./output.txt");
                if (!outputFile.is_open()) {
                    std::cerr << "Failed to open the file for writing." << std::endl;
                    return 1;
                }

                // Write summary (Frobenius norm + factor labels for downstream use)
                outputFile << frobenius << std::endl;
                for (int k = 0; k < factors.numFactors; k++) {
                    outputFile << factors.factorLabels[k] << ":" << factors.labelConfidences[k] << std::endl;
                }
                outputFile.close();

                std::cout << "=========================================" << std::endl;
                std::cout << "✓ Results written to output.txt" << std::endl;
                std::cout << "=========================================" << std::endl;

            } catch (const std::exception& e) {
                std::cerr << "Error in Phase 1 analysis: " << e.what() << std::endl;
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
