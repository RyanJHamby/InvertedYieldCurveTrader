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
#include "DataProcessors/InvertedYieldDataProcessor.hpp"
#include "DataProcessors/CovarianceCalculator.hpp"
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
        InvertedYieldDataProcessor invertedYieldDataProcessor;
        
        std::vector<double> inflationResult = inflationDataProcessor.process();
        std::vector<double> yieldResult = invertedYieldDataProcessor.process();
        
        CovarianceCalculator covarianceCalculator();
        
        covarianceCalculator().calculateCovarianceWithInvertedYield(<#std::vector<double> kpiValues#>, <#std::vector<double> invertedYieldValues#>, std::tuple<double, double> kpiMeanAndStdDev, std::tuple<double, double> invertedYieldMeanAndStdDev)
//        double inflationAndInvertedYieldCovariance = covarianceCalculator.calculateCovariance()
        
//        Aws::Athena::AthenaClient athenaClient(clientConfig);
//        Aws::String query = "SELECT * FROM your_database.your_table LIMIT 10"; // Replace with your query
//
//        Aws::Athena::Model::StartQueryExecutionRequest startRequest;
//        startRequest.SetQueryString(query);
//        startRequest.SetResultConfiguration({
//            Aws::Athena::Model::ResultConfiguration()
//                .WithOutputLocation("/your/s3/output/location")
//        });
//
//        std::cout << "running athena" << std::endl;
//
//        auto startOutcome = athenaClient.StartQueryExecution(startRequest);
//
//        if (startOutcome.IsSuccess()) {
//            Aws::String queryExecutionId = startOutcome.GetResult().GetQueryExecutionId();
//            // You can use queryExecutionId to check the status and retrieve results later
//            std::cout << "Query execution ID: " << queryExecutionId << std::endl;
//        } else {
//            std::cout << "Failed to start query execution: " << startOutcome.GetError().GetMessage() << std::endl;
//        }
        
        
    }

    Aws::ShutdownAPI(options); // Should only be called once.
    return result;
}
