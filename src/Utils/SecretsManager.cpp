//
//  SecretsManager.cpp
//  InvertedYieldCurveTrader
//
//  Created by Ryan Hamby on 12/25/25.
//

#include "SecretsManager.hpp"
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>

// Static member initialization
std::map<std::string, std::string> SecretsManager::secretCache_;
bool SecretsManager::cacheInitialized_ = false;

std::string SecretsManager::getSecret(const std::string& secretName) {
    // Check cache first
    if (cacheInitialized_ && secretCache_.count(secretName)) {
        return secretCache_[secretName];
    }

    std::string value;

    // Try Secrets Manager first (production)
    if (isRunningInLambda()) {
        try {
            value = getFromSecretsManager(secretName);
            secretCache_[secretName] = value;
            return value;
        } catch (const std::exception& e) {
            std::cerr << "Warning: Failed to fetch from Secrets Manager: "
                      << e.what() << " (trying environment variables)" << std::endl;
        }
    }

    // Fall back to environment variables (development)
    try {
        value = getFromEnvironment(secretName);
        secretCache_[secretName] = value;
        return value;
    } catch (const std::exception& e) {
        throw std::runtime_error(
            "Secret '" + secretName + "' not found in Secrets Manager or environment variables: " +
            e.what()
        );
    }
}

std::map<std::string, std::string> SecretsManager::getAllSecrets() {
    std::map<std::string, std::string> secrets;

    // Required secrets for the system
    std::vector<std::string> requiredSecrets = {
        "fred_api_key",
        "alpha_vantage_api_key"
    };

    for (const auto& secretName : requiredSecrets) {
        try {
            secrets[secretName] = getSecret(secretName);
        } catch (const std::exception& e) {
            throw std::runtime_error(
                "Missing required secret: " + secretName + " - " + e.what()
            );
        }
    }

    return secrets;
}

bool SecretsManager::isRunningInLambda() {
    // Lambda sets this environment variable
    return std::getenv("AWS_LAMBDA_FUNCTION_NAME") != nullptr;
}

std::string SecretsManager::getFromSecretsManager(const std::string& secretName) {
    // In production, this would use AWS SDK to fetch from Secrets Manager
    // For now, we'll stub this and rely on environment variables
    //
    // To implement properly:
    // 1. Add aws-secretsmanager service to CMakeLists.txt
    // 2. Use SecretsManagerClient to GetSecretValue
    // 3. Parse JSON response
    //
    // Example:
    /*
    #include <aws/secretsmanager/SecretsManagerClient.h>
    #include <aws/secretsmanager/model/GetSecretValueRequest.h>
    #include <nlohmann/json.hpp>

    Aws::SecretsManager::SecretsManagerClient smClient;
    Aws::SecretsManager::Model::GetSecretValueRequest request;
    request.SetSecretId("inverted-yield-trader/prod");

    auto outcome = smClient.GetSecretValue(request);
    if (outcome.IsSuccess()) {
        auto json = nlohmann::json::parse(outcome.GetResult().GetSecretString());
        return json[secretName].get<std::string>();
    } else {
        throw std::runtime_error(outcome.GetError().GetMessage());
    }
    */

    // For now, delegate to environment variables
    return getFromEnvironment(secretName);
}

std::string SecretsManager::getFromEnvironment(const std::string& secretName) {
    std::string envVarName = toEnvironmentVarName(secretName);
    const char* value = std::getenv(envVarName.c_str());

    if (!value) {
        throw std::runtime_error(
            "Environment variable '" + envVarName + "' not set"
        );
    }

    return std::string(value);
}

std::string SecretsManager::toEnvironmentVarName(const std::string& secretName) {
    std::string result = secretName;

    // Convert to uppercase
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::toupper(c); });

    return result;
}
