//
//  PortfolioRiskAnalyzer.hpp
//  InvertedYieldCurveTrader
//
//  Portfolio risk decomposition by macro factors.
//  Explains portfolio drawdowns via economic causes.
//
//  Created by Ryan Hamby on 12/25/25.
//

#ifndef PORTFOLIO_RISK_ANALYZER_HPP
#define PORTFOLIO_RISK_ANALYZER_HPP

#include "MacroFactorModel.hpp"
#include <Eigen/Dense>
#include <vector>
#include <string>
#include <map>

/**
 * RiskDecomposition: Complete risk attribution for a portfolio
 *
 * Given portfolio sensitivities β and factor model Σ_f,
 * explains portfolio variance as sum of factor risk contributions.
 *
 * Math:
 *   r_t = γ^T f_t + noise     (portfolio returns from factor shocks)
 *   γ = B^T β                 (factor sensitivities)
 *   Var(r) = γ^T Σ_f γ        (total portfolio variance)
 *   RC_k = γ_k (Σ_f γ)_k      (risk contribution of factor k)
 */
struct RiskDecomposition {
    // Factor sensitivities: γ_k = how much portfolio moves per unit shock in factor k
    std::vector<double> factorSensitivities;  // γ_k (K×1)

    // Risk contributions: RC_k = γ_k (Σ_f γ)_k
    // Interpretation: how much variance comes from factor k
    std::vector<double> factorRiskContributions;  // RC_k (K×1)

    // Marginal contribution: ∂Var(r)/∂γ_k = 2 (Σ_f γ)_k
    // Useful for understanding what happens if we increase/decrease exposure
    std::vector<double> marginalRiskContributions;  // MRC_k (K×1)

    // Component contribution: each factor's % of total risk
    std::vector<double> componentContributions;  // RC_k / Total (%)

    // Factor labels and data
    std::vector<std::string> factorLabels;

    // Total portfolio variance
    double totalRisk;
    double totalVariance;  // = totalRisk^2

    // Residual variance (unexplained by factors)
    double residualRisk;

    // How much variance the factors explain
    double varianceExplained;  // = sum(RC_k) / totalVariance

    // Metadata
    int numFactors;
};

/**
 * PortfolioRiskAnalyzer: Exact factor-based risk attribution
 *
 * Core formulas:
 *   Portfolio return: r_t = β^T ε_t = (β^T B) f_t + residual
 *   Factor sensitivities: γ = B^T β
 *   Risk contribution: RC_k = γ_k (Σ_f γ)_k
 *   Total: Σ_k RC_k = γ^T Σ_f γ = portfolio variance
 */
class PortfolioRiskAnalyzer {
public:
    /**
     * Decompose portfolio variance into factor contributions
     *
     * Given portfolio's exposure to macro indicators (β) and factor model,
     * compute how much risk comes from each factor.
     *
     * @param assetSensitivities: β ∈ ℝ^N (exposure to each indicator)
     * @param factors: B, Σ_f from MacroFactorModel
     * @return Risk decomposition with RC_k for each factor
     */
    static RiskDecomposition analyzeRisk(
        const Eigen::VectorXd& assetSensitivities,  // β (N×1)
        const MacroFactors& factors                 // B (N×K), Σ_f (K×K)
    );

    /**
     * Scenario analysis: what if we had a macro shock?
     *
     * Given a hypothetical shock vector f_t, compute portfolio impact.
     * Example: "If growth factor drops 2σ, we lose __% of AUM"
     *
     * @param assetSensitivities: β (portfolio exposures)
     * @param shockVector: hypothetical factor shock (K×1)
     * @param factors: MacroFactors with loadings B
     * @return Portfolio P&L from this shock
     */
    static double scenarioShockImpact(
        const Eigen::VectorXd& assetSensitivities,
        const Eigen::VectorXd& shockVector,
        const MacroFactors& factors
    );

    /**
     * Historical crisis decomposition
     *
     * Explain a past drawdown (e.g., March 2020: -35% ES)
     * by attributing losses to specific macro factors.
     *
     * Given actual portfolio returns and concurrent factor shocks,
     * use regression to find γ and compute RC_k.
     *
     * @param portfolioReturns: Actual returns during crisis period
     * @param factorShocks: Macro factor realizations (T × K)
     * @param factorLabels: Names of factors
     * @return Risk decomposition showing which factors caused loss
     */
    static RiskDecomposition explainHistoricalDrawdown(
        const std::vector<double>& portfolioReturns,
        const std::vector<std::vector<double>>& factorShocks,
        const std::vector<std::string>& factorLabels
    );

    /**
     * Rolling risk decomposition over time
     *
     * Compute risk attribution in rolling windows.
     * Useful for: "Was our growth exposure stable or did it spike?"
     *
     * @param assetSensitivities: β (fixed or time-varying)
     * @param factorsOverTime: MacroFactors for each time window
     * @param windowMonths: Size of rolling window
     * @return Vector of RiskDecomposition, one per window
     */
    static std::vector<RiskDecomposition> rollingRiskDecomposition(
        const Eigen::VectorXd& assetSensitivities,
        const std::vector<MacroFactors>& factorsOverTime,
        int windowMonths = 12
    );

    /**
     * Risk-budgeting optimization
     *
     * Given target portfolio risk and factor volatilities,
     * compute optimal factor sensitivities (γ*) to hit target with min tracking error.
     *
     * Simple version: target_risk = sqrt(γ^T Σ_f γ)
     * Solve for γ such that we hit target_risk with diversification bonus.
     *
     * @param targetRiskLevel: Target portfolio volatility
     * @param factors: Factor volatilities (Σ_f)
     * @param factorLabels: Names for readability
     * @return Recommended γ to hit target risk level
     */
    static RiskDecomposition riskBudgetingAllocation(
        double targetRiskLevel,
        const MacroFactors& factors
    );

private:
    /**
     * Regress portfolio returns on factor shocks via OLS
     *
     * Used in explainHistoricalDrawdown() to recover γ from data
     *
     * @param portfolioReturns: Y (T×1)
     * @param factorShocks: X (T×K)
     * @return Estimated γ and R²
     */
    static std::pair<Eigen::VectorXd, double> regressRiskDecomposition(
        const std::vector<double>& portfolioReturns,
        const std::vector<std::vector<double>>& factorShocks
    );
};

#endif // PORTFOLIO_RISK_ANALYZER_HPP
