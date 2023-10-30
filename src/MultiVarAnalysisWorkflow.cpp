//
//  MultiVarAnalysisWorkflow.cpp
//  InvertedYieldCurveTrader
//
//  Created by Ryan Hamby on 8/27/23.
//

#include <aws/core/Aws.h>
#include <aws/athena/AthenaClient.h>
#include <iostream>
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
using namespace Aws;
using namespace Aws::Auth;

int main(int argc, char **argv) {
    
//    options.loggingOptions.logLevel = Utils::Logging::LogLevel::Debug;
    Aws::SDKOptions options;
    Aws::InitAPI(options); // Should only be called once.
    
    int result = 0;
    
    {
        Aws::Client::ClientConfiguration clientConfig;
        clientConfig.region = "us-east-1";
               
        auto provider = Aws::MakeShared<DefaultAWSCredentialsProviderChain>("alloc-tag");
        auto creds = provider->GetAWSCredentials();
        if (creds.IsEmpty()) {
            std::cerr << "Failed authentication" << std::endl;
        }
        
        InflationDataProcessor inflationDataProcessor;
        GDPDataProcessor gdpDataProcessor;
        InterestRateDataProcessor interestRateDataProcessor;
        InvertedYieldDataProcessor invertedYieldDataProcessor;
        
        std::vector<double> inflationResult = inflationDataProcessor.process();
        std::vector<double> gdpResult = gdpDataProcessor.process();
        std::vector<double> interestRateResult = interestRateDataProcessor.process();
        invertedYieldDataProcessor.process();
        std::vector<double> yieldResult = invertedYieldDataProcessor.getRecentValues();
        double yieldMean = invertedYieldDataProcessor.getMean();
        
        StatsCalculator statsCalculator;
        InvertedYieldStatsCalculator invertedYieldStatsCalculator;
        CovarianceCalculator covarianceCalculator;
        
        double inflationKpiMean = statsCalculator.calculateMean(inflationResult);
        double gdpKpiMean = statsCalculator.calculateMean(gdpResult);
        double interestRateKpiMean = statsCalculator.calculateMean(interestRateResult);
        double invertedYieldMean = invertedYieldStatsCalculator.calculateMean(yieldResult);
        
        double inflationCovariance = covarianceCalculator.calculateCovarianceWithInvertedYield(inflationResult, yieldResult, inflationKpiMean, invertedYieldMean);
        double gdpCovariance = covarianceCalculator.calculateCovarianceWithInvertedYield(gdpResult, yieldResult, gdpKpiMean, invertedYieldMean);
        double interestRateCovariance = covarianceCalculator.calculateCovarianceWithInvertedYield(interestRateResult, yieldResult, interestRateKpiMean, invertedYieldMean);
        
        std::cout << inflationCovariance << std::endl;
        std::cout << gdpCovariance << std::endl;
        std::cout << interestRateCovariance << std::endl;
    }

    Aws::ShutdownAPI(options); // Should only be called once.
    return result;
}
