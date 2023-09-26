//
//  S3ObjectRetriever.hpp
//  InvertedYieldCurveTrader
//
//  Created by Ryan Hamby on 9/21/23.
//

#ifndef S3_OBJECT_RETRIEVER_HPP
#define S3_OBJECT_RETRIEVER_HPP

#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <aws/core/utils/Outcome.h>
#include <string>

class S3ObjectRetriever {
public:
    S3ObjectRetriever(const std::string& bucketName, const std::string& objectKey);
    ~S3ObjectRetriever();

    bool Initialize();
    bool RetrieveJson(std::string& jsonData);

private:
    Aws::SDKOptions options;
    Aws::S3::S3Client s3Client;
    Aws::S3::Model::GetObjectRequest getObjectRequest;
    std::string bucketName;
    std::string objectKey;
};

#endif
