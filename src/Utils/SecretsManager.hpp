//
//  SecretsManager.hpp
//  InvertedYieldCurveTrader
//
//  Manages API keys via AWS Secrets Manager (production)
//  Falls back to environment variables (local development)
//
//  Created by Ryan Hamby on 12/25/25.
//

#ifndef SECRETS_MANAGER_HPP
#define SECRETS_MANAGER_HPP

#include <string>
#include <map>
#include <stdexcept>

/**
 * Secure credential management
 *
 * In production (Lambda): Reads from AWS Secrets Manager
 * In development (local): Falls back to environment variables
 *
 * Usage:
 *   SecretsManager secrets;
 *   std::string fredKey = secrets.getSecret("fred_api_key");
 */
class SecretsManager {
public:
    /**
     * Get a secret value
     *
     * @param secretName: Secret name (e.g., "fred_api_key", "alpha_vantage_key")
     * @return Secret value
     * @throws std::runtime_error if secret not found in either source
     */
    static std::string getSecret(const std::string& secretName);

    /**
     * Get all secrets needed for the system
     *
     * @return Map of {name: value} for all required secrets
     * @throws std::runtime_error if any required secret is missing
     */
    static std::map<std::string, std::string> getAllSecrets();

    /**
     * Check if running in AWS Lambda environment
     *
     * @return true if AWS_LAMBDA_FUNCTION_NAME env var is set
     */
    static bool isRunningInLambda();

private:
    // Cache to avoid repeated Secrets Manager calls
    static std::map<std::string, std::string> secretCache_;
    static bool cacheInitialized_;

    /**
     * Fetch from AWS Secrets Manager
     *
     * @param secretName: Secret name
     * @return Secret value
     * @throws std::runtime_error if not found
     */
    static std::string getFromSecretsManager(const std::string& secretName);

    /**
     * Fetch from environment variables
     *
     * @param secretName: Environment variable name (converted to uppercase)
     * @return Secret value
     * @throws std::runtime_error if not found
     */
    static std::string getFromEnvironment(const std::string& secretName);

    /**
     * Convert secret name to env var name
     * Example: "fred_api_key" → "FRED_API_KEY"
     */
    static std::string toEnvironmentVarName(const std::string& secretName);
};

#endif // SECRETS_MANAGER_HPP
