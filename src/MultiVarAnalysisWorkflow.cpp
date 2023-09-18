//
//  main.cpp
//  InvertedYieldCurveTrader
//
//  Created by Ryan Hamby on 8/27/23.
//

#include <aws/core/Aws.h>
#include <aws/athena/AthenaClient.h>
#include <iostream>
#include <aws/core/auth/AWSCredentialsProviderChain.h>
using namespace Aws;
using namespace Aws::Auth;

int main(int argc, char **argv) {
    Aws::SDKOptions options;
    // Optionally change the log level for debugging.
//   options.loggingOptions.logLevel = Utils::Logging::LogLevel::Debug;
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

//        Aws::S3::S3Client s3Client(clientConfig);
//        auto outcome = s3Client.ListBuckets();

//        if (!outcome.IsSuccess()) {
//            std::cerr << "Failed with error: " << outcome.GetError() << std::endl;
//            result = 1;
//        } else {
//            std::cout << "Found " << outcome.GetResult().GetBuckets().size()
//                      << " buckets\n";
//            for (auto &bucket: outcome.GetResult().GetBuckets()) {
//                std::cout << bucket.GetName() << std::endl;
//            }
//        }
    }

    Aws::ShutdownAPI(options); // Should only be called once.
    return result;
}
