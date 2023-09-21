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
#include <aws/core/auth/AWSCredentialsProvider.h>
#include <iostream>

S3ObjectRetriever::S3ObjectRetriever(const std::string& bucketName, const std::string& objectKey)
    : options(), s3Client(Aws::Auth::AWSCredentialsProvider::GetDefaultAWSCredentialsProvider(), options),
      getObjectRequest(), bucketName(bucketName), objectKey(objectKey) {}

S3ObjectRetriever::~S3ObjectRetriever() {
    Aws::ShutdownAPI(options);
}

bool S3ObjectRetriever::Initialize() {
    if (!Aws::InitAPI(options)) {
        std::cerr << "Failed to initialize AWS SDK" << std::endl;
        return false;
    }
    return true;
}

bool S3ObjectRetriever::RetrieveJson(std::string& jsonData) {
    getObjectRequest.SetBucket(bucketName);
    getObjectRequest.SetKey(objectKey);

    auto getObjectOutcome = s3Client.GetObject(getObjectRequest);

    if (getObjectOutcome.IsSuccess()) {
        jsonData = Aws::Utils::Stream::ReadAllToString(getObjectOutcome.GetResult().GetBody().get());
        return true;
    } else {
        std::cerr << "Error retrieving JSON from S3: " << getObjectOutcome.GetError().GetMessage() << std::endl;
        return false;
    }
}

#endif /* S3ObjectRetriever_hpp */
