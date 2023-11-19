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
            
            double traderPrincipal = 1000.0;
            double traderCashAccount = 1000.0;
            
            double marketPrincipal = 2000.0;
            
            double maxPercentTraderPrincipalToSellPerTransaction = 0.02;
            double maxPercentTraderCashAccountToBuyPerTransaction = 0.02;
            
            double startingSharesInMarket = stockDataResult.size() > 0 ? 2000.0 / stockDataResult[0] : 0;
            
            for (int i = 0; i < averageSlidingWindowStockScores.size(); ++i) {
                // combine confidence score found from covariances with per-minute trading stock data
                double predictedMarketDelta = confidenceScore * averageSlidingWindowStockScores[i];
                
                if (predictedMarketDelta > 0) {
                    double amountToSell = traderPrincipal * maxPercentTraderPrincipalToSellPerTransaction * predictedMarketDelta;
                    traderPrincipal -= amountToSell;
                    traderCashAccount += amountToSell;
                }
                else if (predictedMarketDelta < 0) {
                    double amountToBuy = traderCashAccount * maxPercentTraderPrincipalToSellPerTransaction * predictedMarketDelta;
                    traderPrincipal += amountToBuy;
                    traderCashAccount -= amountToBuy;
                }
                double totalPortfolioValueAfterTrade = traderPrincipal + traderCashAccount;
                actualTradingResults.push_back(totalPortfolioValueAfterTrade);
            }
            for (int i = 0; i < actualTradingResults.size(); ++i) {
                std::cout << actualTradingResults[i] - stockDataResult[i] * startingSharesInMarket << std::endl;
            }
        }
    }

    Aws::ShutdownAPI(options); // Should only be called once.
    return result;
}
