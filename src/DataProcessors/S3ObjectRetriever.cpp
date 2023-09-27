//
//  S3ObjectRetriever.cpp
//  InvertedYieldCurveTrader
//
//  Created by Ryan Hamby on 9/21/23.
//

#ifndef S3ObjectRetriever_hpp
#define S3ObjectRetriever_hpp

#include <stdio.h>

#include "S3ObjectRetriever.hpp"
#include <aws/s3/S3Client.h>
#include <aws/core/auth/AWSCredentialsProvider.h>
#include <aws/core/endpoint/DefaultEndpointProvider.h>
#include <iostream>

S3ObjectRetriever::S3ObjectRetriever(const std::string& bucketName, const std::string& objectKey)
    : getObjectRequest(), bucketName(bucketName), objectKey(objectKey) {
        Aws::S3::S3ClientConfiguration clientConfig;
        std::shared_ptr<Aws::S3::S3EndpointProviderBase> endpointProvider = Aws::MakeShared<Aws::S3::S3EndpointProvider>("myEndpointProvider");
        s3Client = Aws::S3::S3Client(clientConfig, endpointProvider);
    }

bool S3ObjectRetriever::RetrieveJson(std::string& jsonData) {
    getObjectRequest.SetBucket(bucketName);
    getObjectRequest.SetKey(objectKey);

    auto getObjectOutcome = s3Client.GetObject(getObjectRequest);

    if (getObjectOutcome.IsSuccess()) {
        jsonData = getObjectOutcome.GetResult().GetBody().get();
        return true;
    } else {
        std::cerr << "Error retrieving JSON from S3: " << getObjectOutcome.GetError().GetMessage() << std::endl;
        return false;
    }
}

#endif /* S3ObjectRetriever_hpp */
