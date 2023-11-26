//
//  DynamoDBClient.cpp
//  InvertedYieldCurveTrader
//
//  Created by Ryan Hamby on 11/23/23.
//

#include "DynamoDBClient.hpp"
#include <aws/dynamodb/DynamoDBClient.h>
#include <aws/dynamodb/model/PutItemRequest.h>
#include <aws/dynamodb/model/GetItemRequest.h>
#include "../Utils/Date.hpp"

DynamoDBClient::DynamoDBClient(Aws::Client::ClientConfiguration clientConfig) {
    Aws::DynamoDB::DynamoDBClient dynamoClient(clientConfig);
};

double DynamoDBClient::getDoubleItem(std::string tableNameString,
                                 std::string pkColumn,
                                 std::string pkValuePrefix,
                                 std::string attributeColumn) {
    const char* tableName = tableNameString.c_str();

    Aws::DynamoDB::Model::AttributeValue keyAttributeValue;
    keyAttributeValue.SetS(pkValuePrefix + getDateDaysAgo(1));

    Aws::DynamoDB::Model::GetItemRequest getItemRequest;
    getItemRequest.SetTableName(tableName);
    getItemRequest.AddKey(pkColumn, keyAttributeValue);

    auto getItemOutcome = dynamoClient.GetItem(getItemRequest);

    if (getItemOutcome.IsSuccess()) {
        const auto& item = getItemOutcome.GetResult().GetItem();
        if (!item.empty()) {
            const auto& attributeValue = item.find(attributeColumn)->second;
            return std::stod(attributeValue.GetN());
        } else {
            std::cout << "Item not found." << std::endl;
        }
    } else {
        std::cerr << "Error: " << getItemOutcome.GetError().GetMessage() << std::endl;
    }
};

void DynamoDBClient::putDailyResultItem(Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> item,
                                        std::string tableNameString) {
    Aws::DynamoDB::Model::PutItemRequest putItemRequest;
    putItemRequest.SetTableName(tableNameString);

    for (const auto& pair : item) {
        putItemRequest.AddItem(pair.first, pair.second);
    }

    auto outcome = dynamoClient.PutItem(putItemRequest);

    if (outcome.IsSuccess()) {
        std::cout << "Item successfully added to DynamoDB" << std::endl;
    } else {
        std::cerr << "Failed to put item into DynamoDB: " << outcome.GetError().GetMessage() << std::endl;
    }
};
