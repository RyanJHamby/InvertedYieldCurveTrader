//
//  PortfolioRiskAnalyzer.cpp
//  InvertedYieldCurveTrader
//
//  Implementation of exact portfolio risk decomposition by macro factors
//
//  Created by Ryan Hamby on 12/25/25.
//

#include "PortfolioRiskAnalyzer.hpp"
#include <cmath>
#include <numeric>
#include <iostream>
#include <algorithm>

RiskDecomposition PortfolioRiskAnalyzer::analyzeRisk(
    const Eigen::VectorXd& assetSensitivities,
    const MacroFactors& factors)
{
    // Validate inputs
    if (assetSensitivities.size() != factors.indicatorNames.size()) {
        throw std::invalid_argument(
            "Asset sensitivities size must match number of indicators");
    }

    if (factors.numFactors < 1) {
        throw std::invalid_argument("Factors must have at least 1 factor");
    }

    int N = assetSensitivities.size();  // Number of indicators
    int K = factors.numFactors;          // Number of factors

    // Step 1: Compute factor sensitivities γ = B^T β
    // β is assetSensitivities (N×1)
    // B is factors.loadings (N×K)
    // γ = B^T β (K×1)
    Eigen::VectorXd gamma = factors.loadings.transpose() * assetSensitivities;

    // Step 2: Compute portfolio variance = γ^T Σ_f γ
    // Σ_f is diagonal of factorVariances
    Eigen::MatrixXd covarianceF = Eigen::MatrixXd::Zero(K, K);
    for (int k = 0; k < K; k++) {
        covarianceF(k, k) = factors.factorVariances[k];
    }

    // Compute Σ_f γ
    Eigen::VectorXd sigmaGamma = covarianceF * gamma;

    // Portfolio variance: γ^T Σ_f γ
    double portfolioVariance = gamma.dot(sigmaGamma);
    double portfolioRisk = std::sqrt(std::max(0.0, portfolioVariance));

    // Step 3: Compute risk contributions RC_k = γ_k (Σ_f γ)_k
    std::vector<double> riskContributions(K);
    double totalRiskContribution = 0.0;

    for (int k = 0; k < K; k++) {
        riskContributions[k] = gamma(k) * sigmaGamma(k);
        totalRiskContribution += riskContributions[k];
    }

    // Step 4: Compute residual variance from unexplained covariance
    // Residual risk = sqrt(u_t^T E[u_t u_t^T] u_t)
    // Simplified: use Frobenius norm of residual covariance as proxy
    double residualVariance = factors.residualCovariance.norm();
    double residualRisk = residualVariance;

    // Step 5: Compute marginal contributions and component contributions
    std::vector<double> marginalContributions(K);
    std::vector<double> componentContributions(K);

    for (int k = 0; k < K; k++) {
        // Marginal contribution: ∂Var(r)/∂γ_k = 2 (Σ_f γ)_k
        marginalContributions[k] = 2.0 * sigmaGamma(k);

        // Component contribution: percentage of total variance
        if (portfolioVariance > 1e-10) {
            componentContributions[k] = riskContributions[k] / portfolioVariance;
        } else {
            componentContributions[k] = 0.0;
        }
    }

    // Construct result
    RiskDecomposition result;
    result.factorSensitivities = std::vector<double>(gamma.data(), gamma.data() + K);
    result.factorRiskContributions = riskContributions;
    result.marginalRiskContributions = marginalContributions;
    result.componentContributions = componentContributions;
    result.factorLabels = factors.factorLabels;
    result.totalRisk = portfolioRisk;
    result.totalVariance = portfolioVariance;
    result.residualRisk = residualRisk;
    result.varianceExplained = factors.cumulativeVarianceExplained;
    result.numFactors = K;

    return result;
}

double PortfolioRiskAnalyzer::scenarioShockImpact(
    const Eigen::VectorXd& assetSensitivities,
    const Eigen::VectorXd& shockVector,
    const MacroFactors& factors)
{
    // Validate
    if (shockVector.size() != factors.numFactors) {
        throw std::invalid_argument("Shock vector size must match number of factors");
    }

    // Compute factor sensitivities γ = B^T β
    Eigen::VectorXd gamma = factors.loadings.transpose() * assetSensitivities;

    // Portfolio impact from factor shocks: r_scenario = γ^T f
    double impact = gamma.dot(shockVector);

    return impact;
}

RiskDecomposition PortfolioRiskAnalyzer::explainHistoricalDrawdown(
    const std::vector<double>& portfolioReturns,
    const std::vector<std::vector<double>>& factorShocks,
    const std::vector<std::string>& factorLabels)
{
    // Validate inputs
    if (portfolioReturns.empty()) {
        throw std::invalid_argument("Portfolio returns cannot be empty");
    }

    if (factorShocks.empty()) {
        throw std::invalid_argument("Factor shocks cannot be empty");
    }

    if (portfolioReturns.size() != factorShocks.size()) {
        throw std::invalid_argument("Returns and shocks must have same length");
    }

    int T = portfolioReturns.size();
    int K = factorShocks[0].size();

    if (factorLabels.size() != static_cast<size_t>(K)) {
        throw std::invalid_argument("Factor labels size must match shock dimensions");
    }

    // Perform OLS regression: r_t = γ^T f_t + ε_t
    auto [gamma, rsquared] = regressRiskDecomposition(portfolioReturns, factorShocks);

    // Compute historical average factor covariance
    Eigen::MatrixXd covF = Eigen::MatrixXd::Zero(K, K);
    for (int t = 0; t < T; t++) {
        for (int k = 0; k < K; k++) {
            for (int j = 0; j < K; j++) {
                covF(k, j) += factorShocks[t][k] * factorShocks[t][j];
            }
        }
    }
    covF /= T;

    // Compute risk contributions RC_k = γ_k (Σ_f γ)_k
    Eigen::VectorXd sigmaGamma = covF * gamma;
    std::vector<double> riskContributions(K);
    double totalVariance = 0.0;

    for (int k = 0; k < K; k++) {
        riskContributions[k] = gamma(k) * sigmaGamma(k);
        totalVariance += riskContributions[k];
    }

    // Compute component contributions
    std::vector<double> componentContributions(K);
    for (int k = 0; k < K; k++) {
        if (totalVariance > 1e-10) {
            componentContributions[k] = riskContributions[k] / totalVariance;
        } else {
            componentContributions[k] = 0.0;
        }
    }

    // Compute portfolio variance over period
    double portfolioVariance = 0.0;
    double portfolioMeanReturn = std::accumulate(portfolioReturns.begin(),
                                                 portfolioReturns.end(), 0.0) / T;
    for (double r : portfolioReturns) {
        portfolioVariance += (r - portfolioMeanReturn) * (r - portfolioMeanReturn);
    }
    portfolioVariance /= T;
    double portfolioRisk = std::sqrt(std::max(0.0, portfolioVariance));

    // Construct result
    RiskDecomposition result;
    result.factorSensitivities = std::vector<double>(gamma.data(), gamma.data() + K);
    result.factorRiskContributions = riskContributions;
    result.factorLabels = factorLabels;
    result.totalRisk = portfolioRisk;
    result.totalVariance = portfolioVariance;
    result.componentContributions = componentContributions;
    result.varianceExplained = rsquared;
    result.numFactors = K;

    return result;
}

std::vector<RiskDecomposition> PortfolioRiskAnalyzer::rollingRiskDecomposition(
    const Eigen::VectorXd& assetSensitivities,
    const std::vector<MacroFactors>& factorsOverTime,
    int windowMonths)
{
    if (windowMonths < 1) {
        throw std::invalid_argument("Window months must be >= 1");
    }

    if (factorsOverTime.empty()) {
        throw std::invalid_argument("Factors over time cannot be empty");
    }

    std::vector<RiskDecomposition> results;

    for (const auto& factors : factorsOverTime) {
        try {
            RiskDecomposition decomp = analyzeRisk(assetSensitivities, factors);
            results.push_back(decomp);
        } catch (const std::exception& e) {
            // Skip this window if analysis fails
            std::cerr << "Warning: Risk decomposition failed: " << e.what() << std::endl;
            continue;
        }
    }

    return results;
}

RiskDecomposition PortfolioRiskAnalyzer::riskBudgetingAllocation(
    double targetRiskLevel,
    const MacroFactors& factors)
{
    if (targetRiskLevel <= 0.0) {
        throw std::invalid_argument("Target risk level must be positive");
    }

    int K = factors.numFactors;

    // Simple risk budgeting: allocate risk equally across factors
    // γ_k = target_risk / sqrt(K * σ_k^2)
    // This gives equal risk contribution per factor

    std::vector<double> gammas(K);
    double normalizationFactor = 0.0;

    for (int k = 0; k < K; k++) {
        double sigma_k = std::sqrt(factors.factorVariances[k]);
        if (sigma_k > 1e-10) {
            gammas[k] = 1.0 / sigma_k;
            normalizationFactor += gammas[k] * gammas[k] * factors.factorVariances[k];
        } else {
            gammas[k] = 0.0;
        }
    }

    // Scale to achieve target risk
    if (normalizationFactor > 1e-10) {
        double scale = targetRiskLevel / std::sqrt(normalizationFactor);
        for (int k = 0; k < K; k++) {
            gammas[k] *= scale;
        }
    }

    // Compute resulting risk decomposition
    Eigen::VectorXd gamma = Eigen::Map<Eigen::VectorXd>(gammas.data(), K);

    Eigen::MatrixXd covF = Eigen::MatrixXd::Zero(K, K);
    for (int k = 0; k < K; k++) {
        covF(k, k) = factors.factorVariances[k];
    }

    Eigen::VectorXd sigmaGamma = covF * gamma;
    double portfolioVariance = gamma.dot(sigmaGamma);

    std::vector<double> riskContributions(K);
    std::vector<double> componentContributions(K);

    for (int k = 0; k < K; k++) {
        riskContributions[k] = gamma(k) * sigmaGamma(k);
        if (portfolioVariance > 1e-10) {
            componentContributions[k] = riskContributions[k] / portfolioVariance;
        } else {
            componentContributions[k] = 0.0;
        }
    }

    RiskDecomposition result;
    result.factorSensitivities = gammas;
    result.factorRiskContributions = riskContributions;
    result.componentContributions = componentContributions;
    result.factorLabels = factors.factorLabels;
    result.totalRisk = std::sqrt(std::max(0.0, portfolioVariance));
    result.totalVariance = portfolioVariance;
    result.numFactors = K;

    return result;
}

std::pair<Eigen::VectorXd, double> PortfolioRiskAnalyzer::regressRiskDecomposition(
    const std::vector<double>& portfolioReturns,
    const std::vector<std::vector<double>>& factorShocks)
{
    int T = portfolioReturns.size();
    int K = factorShocks[0].size();

    // Build design matrix X (T × K) from factor shocks
    Eigen::MatrixXd X(T, K);
    for (int t = 0; t < T; t++) {
        for (int k = 0; k < K; k++) {
            X(t, k) = factorShocks[t][k];
        }
    }

    // Build response vector y (T × 1)
    Eigen::VectorXd y = Eigen::Map<const Eigen::VectorXd>(
        portfolioReturns.data(), T);

    // OLS: γ = (X^T X)^{-1} X^T y
    Eigen::MatrixXd XtX = X.transpose() * X;
    Eigen::VectorXd Xty = X.transpose() * y;

    // Add small regularization to avoid singularity
    Eigen::MatrixXd XtXReg = XtX;
    for (int k = 0; k < K; k++) {
        XtXReg(k, k) += 1e-8;
    }

    Eigen::VectorXd gamma = XtXReg.ldlt().solve(Xty);

    // Compute R² = 1 - (||y - X γ||^2 / ||y - mean(y)||^2)
    Eigen::VectorXd yPred = X * gamma;
    double meanY = y.mean();
    double ssRes = (y - yPred).squaredNorm();
    double ssTot = (y.array() - meanY).square().sum();

    double rsquared = 1.0;
    if (ssTot > 1e-10) {
        rsquared = 1.0 - (ssRes / ssTot);
        rsquared = std::max(0.0, std::min(1.0, rsquared));  // Clamp to [0,1]
    }

    return {gamma, rsquared};
}
