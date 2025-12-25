//
//  MacroFactorModel.hpp
//  InvertedYieldCurveTrader
//
//  PCA-based decomposition of macro shock covariance into interpretable factors.
//  Uses economic archetypes for robust, stable factor labeling.
//
//  Created by Ryan Hamby on 12/25/25.
//

#ifndef MACRO_FACTOR_MODEL_HPP
#define MACRO_FACTOR_MODEL_HPP

#include "CovarianceCalculator.hpp"
#include <Eigen/Dense>
#include <vector>
#include <string>
#include <map>
#include <stdexcept>

/**
 * LabelResult: Result of factor labeling with confidence and stability tracking
 */
struct LabelResult {
    std::string label;              // "Growth", "Inflation", "Policy", "Volatility", or "Unclassified"
    double cosineScore;             // 0.0-1.0, confidence in label
    bool isStable;                  // true if label matches previous window
    std::string message;            // Diagnostic message
};

/**
 * MacroFactors: Complete factor decomposition result
 */
struct MacroFactors {
    std::vector<Eigen::VectorXd> factors;           // Factor time series (K factors × T observations)
    Eigen::MatrixXd loadings;                       // B matrix: indicator × factor loadings (N × K)
    std::vector<double> factorVariances;            // Var(f_k) for each factor
    std::vector<std::string> factorLabels;          // "Growth", "Inflation", "Policy", "Volatility"
    std::vector<double> labelConfidences;           // Cosine score (0.0-1.0) for each label
    double cumulativeVarianceExplained;             // % of total covariance explained
    Eigen::MatrixXd residualCovariance;             // Σ_u: unexplained covariance

    // Metadata
    int numFactors;
    std::vector<std::string> indicatorNames;        // Original indicator names
};

/**
 * MacroFactorModel: PCA-based factor decomposition with robust labeling
 *
 * Key principle: Separates statistical factors (from PCA) from economic interpretation
 * (via archetype matching). Detects and alerts on label instability.
 */
class MacroFactorModel {
public:
    /**
     * Decompose surprise covariance using PCA
     *
     * @param surpriseCov: 8x8 covariance matrix of macro surprises
     * @param numFactors: Number of factors to extract (default 3)
     * @return MacroFactors struct with loadings, variances, labels
     */
    static MacroFactors decomposeSurpriseCovariance(
        const CovarianceMatrix& surpriseCov,
        int numFactors = 3
    );

    /**
     * Label a factor robustly using cosine similarity to economic archetypes
     *
     * NOT post-hoc heuristics. Instead, match loading vector to fixed archetypes.
     *
     * @param loading: Column of loadings matrix (one factor)
     * @param indicatorNames: Names of indicators (must be sorted)
     * @param previousLabel: Label from previous window (for stability check)
     * @return LabelResult with label, confidence, stability flag
     */
    static LabelResult labelFactorRobustly(
        const Eigen::VectorXd& loading,
        const std::vector<std::string>& indicatorNames,
        const LabelResult& previousLabel = LabelResult{"", 0.0, true, ""}
    );

    /**
     * Rolling-window decomposition with drift detection
     *
     * Decomposes surprise covariance in rolling windows, detects when factors
     * become unstable (labels flip), and flags for manual review.
     *
     * @param surprises: Map of indicator → surprise time series
     * @param windowMonths: Size of rolling window (default 12)
     * @param numFactors: Number of factors (default 3)
     * @param stabilityThreshold: Cosine similarity threshold for stability (default 0.85)
     * @return Vector of MacroFactors, one per window
     */
    static std::vector<MacroFactors> rollingDecompositionWithDriftDetection(
        const std::map<std::string, std::vector<double>>& surprises,
        int windowMonths = 12,
        int numFactors = 3,
        double stabilityThreshold = 0.85
    );

    /**
     * Get economic archetype vectors for labeling
     *
     * These are FIXED reference vectors representing:
     * - Growth: GDP↑, unemployment↓, sentiment↑
     * - Inflation: CPI↑, PCE↑
     * - Policy: Fed funds↑, spreads↑
     * - Volatility: VIX↑, MOVE↑
     *
     * @return Map of archetype_name → reference vector
     */
    static std::map<std::string, Eigen::VectorXd> getEconomicArchetypes(
        const std::vector<std::string>& indicatorNames
    );

private:
    /**
     * Compute cosine similarity between two vectors
     *
     * @param v1: First vector
     * @param v2: Second vector
     * @return Cosine similarity (0 to 1)
     */
    static double cosineSimilarity(
        const Eigen::VectorXd& v1,
        const Eigen::VectorXd& v2
    );
};

#endif // MACRO_FACTOR_MODEL_HPP
