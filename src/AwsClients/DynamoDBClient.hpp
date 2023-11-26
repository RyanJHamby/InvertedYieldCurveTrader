//
//  DynamoDBClient.hpp
//  InvertedYieldCurveTrader
//
//  Created by Ryan Hamby on 11/23/23.
//

#ifndef DynamoDBClient_hpp
#define DynamoDBClient_hpp

#include <stdio.h>
#include <aws/core/auth/AWSCredentialsProviderChain.h>
#include <aws/dynamodb/DynamoDBClient.h>

class DynamoDBClient {
public:
    DynamoDBClient(Aws::Client::ClientConfiguration clientConfig);
    double getDoubleItem(std::string tableNameString,
                     std::string pkColumn,
                     std::string pkValuePrefix,
                     std::string attributeColumn);
    void putDailyResultItem(Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> item,
                            std::string tableNameString);
    
private:
    Aws::DynamoDB::DynamoDBClient dynamoClient;
};

#endif /* DynamoDBClient_hpp */
