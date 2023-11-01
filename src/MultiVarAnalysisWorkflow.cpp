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
#include "DataProcessors/StockDataRetriever.hpp"
using namespace Aws;
using namespace Aws::Auth;

int main(int argc, char **argv) {
    
//    options.loggingOptions.logLevel = Utils::Logging::LogLevel::Debug;
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
            /// Will only use inflation data for the time being, due to API throttling
    //        GDPDataProcessor gdpDataProcessor;
    //        InterestRateDataProcessor interestRateDataProcessor;

    //        std::vector<double> gdpResult = gdpDataProcessor.process();
    //        std::vector<double> interestRateResult = interestRateDataProcessor.process();
    //        double yieldMean = invertedYieldDataProcessor.getMean();

    //        double gdpKpiMean = statsCalculator.calculateMean(gdpResult);
    //        double interestRateKpiMean = statsCalculator.calculateMean(interestRateResult);

    //        double gdpCovariance = covarianceCalculator.calculateCovarianceWithInvertedYield(gdpResult, yieldResult, gdpKpiMean, invertedYieldMean);
    //        double interestRateCovariance = covarianceCalculator.calculateCovarianceWithInvertedYield(interestRateResult, yieldResult, interestRateKpiMean, invertedYieldMean);

    //        std::cout << gdpCovariance << std::endl;
    //        std::cout << interestRateCovariance << std::endl;
            
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
            
            std::cout << inflationCovariance << std::endl;
            double averageMagnitudeOfCovariance = (abs(inflationCovariance));
            double confidenceToBuy =  statsCalculator.calculateCovarianceConfidence(averageMagnitudeOfCovariance);
            
            // Need to write intermediate result to output file to avoid API throttling, then will rerun with different instrumentation after a minute
            std::ofstream outputFile("./output.txt");
            if (!outputFile.is_open()) {
                std::cerr << "Failed to open the file for writing." << std::endl;
                return 1;
            }
            
            outputFile << confidenceToBuy << std::endl;
            outputFile.close();
        } else if (std::strcmp(argv[1], "trade") == 0) {
            StockDataRetriever stockDataRetriever;
            std::cout << "trading" << std::endl;
            std::vector<double> stockDataResult = stockDataRetriever.retrieve();
            for (auto &i : stockDataResult) {
                std::cout << i << std::endl;
            }

            // call APIs for time series
        }
    }

    Aws::ShutdownAPI(options); // Should only be called once.
    return result;
}
