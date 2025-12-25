//
//  MacroFactorModel.cpp
//  InvertedYieldCurveTrader
//
//  PCA-based macro factor decomposition with robust economic labeling
//
//  Created by Ryan Hamby on 12/25/25.
//

#include "MacroFactorModel.hpp"
#include <Eigen/Eigenvalues>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>

MacroFactors MacroFactorModel::decomposeSurpriseCovariance(
    const CovarianceMatrix& surpriseCov,
    int numFactors)
{
    if (numFactors < 1) {
        throw std::invalid_argument("numFactors must be >= 1");
    }

    Eigen::MatrixXd cov = surpriseCov.getMatrix();
    auto indicatorNames = surpriseCov.getIndicatorNames();

    int totalIndicators = cov.rows();
    if (numFactors > totalIndicators) {
        throw std::invalid_argument("numFactors cannot exceed number of indicators");
    }

    // Compute eigendecomposition
    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> solver(cov);

    if (solver.info() != Eigen::Success) {
        throw std::runtime_error("Eigendecomposition failed");
    }

    Eigen::VectorXd eigenvalues = solver.eigenvalues();
    Eigen::MatrixXd eigenvectors = solver.eigenvectors();

    // Sort by descending eigenvalues (largest variance first)
    std::vector<int> indices(totalIndicators);
    std::iota(indices.begin(), indices.end(), 0);
    std::sort(indices.begin(), indices.end(), [&eigenvalues](int a, int b) {
        return eigenvalues(a) > eigenvalues(b);
    });

    // Extract top K factors
    Eigen::MatrixXd topEigenvectors(totalIndicators, numFactors);
    Eigen::VectorXd topEigenvalues(numFactors);

    for (int k = 0; k < numFactors; k++) {
        int idx = indices[k];
        topEigenvectors.col(k) = eigenvectors.col(idx);
        topEigenvalues(k) = eigenvalues(idx);
    }

    // Loadings: B = U * sqrt(Λ)
    Eigen::MatrixXd loadings = topEigenvectors * topEigenvalues.cwiseSqrt().asDiagonal();

    // Compute factor variances
    std::vector<double> factorVariances(numFactors);
    for (int k = 0; k < numFactors; k++) {
        factorVariances[k] = topEigenvalues(k);
    }

    // Cumulative variance explained
    double totalVariance = eigenvalues.sum();
    double explainedVariance = topEigenvalues.sum();
    double cumulativeVarExplained = 0.0;
    if (totalVariance > 1e-10) {
        cumulativeVarExplained = explainedVariance / totalVariance;
    }

    // Label factors
    std::vector<std::string> labels;
    std::vector<double> confidences;
    for (int k = 0; k < numFactors; k++) {
        LabelResult result = labelFactorRobustly(loadings.col(k), indicatorNames);
        labels.push_back(result.label);
        confidences.push_back(result.cosineScore);
    }

    // Residual covariance: Σ_u = Σ - B B^T
    Eigen::MatrixXd residualCov = cov - loadings * loadings.transpose();

    // Construct result
    MacroFactors result;
    result.loadings = loadings;
    result.factorVariances = factorVariances;
    result.factorLabels = labels;
    result.labelConfidences = confidences;
    result.cumulativeVarianceExplained = cumulativeVarExplained;
    result.residualCovariance = residualCov;
    result.numFactors = numFactors;
    result.indicatorNames = indicatorNames;

    // Note: factors time series would be computed when applied to actual data
    // For now, this is just the structural decomposition

    return result;
}

LabelResult MacroFactorModel::labelFactorRobustly(
    const Eigen::VectorXd& loading,
    const std::vector<std::string>& indicatorNames,
    const LabelResult& previousLabel)
{
    // Get archetypes for these indicators
    auto archetypes = getEconomicArchetypes(indicatorNames);

    if (archetypes.empty()) {
        return {"Unclassified", 0.0, true, "No archetypes available"};
    }

    // Compute cosine similarity to each archetype
    std::map<std::string, double> scores;
    for (auto& [name, archetype] : archetypes) {
        double score = cosineSimilarity(loading, archetype);
        scores[name] = score;
    }

    // Find best match
    auto bestMatch = std::max_element(
        scores.begin(),
        scores.end(),
        [](const auto& a, const auto& b) { return a.second < b.second; }
    );

    std::string label = bestMatch->first;
    double bestScore = bestMatch->second;

    // Check stability: did label change from previous window?
    bool isStable = true;
    std::string message = "";

    if (!previousLabel.label.empty() && previousLabel.label != "Unclassified") {
        if (label != previousLabel.label) {
            isStable = false;
            message = "Label changed from " + previousLabel.label + " to " + label;
        }
    }

    // Apply confidence threshold: if best score < 0.65, mark as Unclassified
    if (bestScore < 0.65) {
        label = "Unclassified";
        message = "Best archetype match score " + std::to_string(bestScore) + " < 0.65 threshold";
        isStable = false;
    }

    return {label, bestScore, isStable, message};
}

std::vector<MacroFactors> MacroFactorModel::rollingDecompositionWithDriftDetection(
    const std::map<std::string, std::vector<double>>& surprises,
    int windowMonths,
    int numFactors,
    double stabilityThreshold)
{
    if (windowMonths < 1) {
        throw std::invalid_argument("windowMonths must be >= 1");
    }

    // Verify all surprise series have same length
    if (surprises.empty()) {
        throw std::invalid_argument("surprises map cannot be empty");
    }

    size_t timeSeriesLength = surprises.begin()->second.size();
    for (const auto& [ind, series] : surprises) {
        if (series.size() != timeSeriesLength) {
            throw std::invalid_argument("All surprise series must have same length");
        }
    }

    std::vector<MacroFactors> results;
    LabelResult previousLabel{"", 0.0, true, ""};

    // Rolling window: slide through time
    for (size_t t = windowMonths; t <= timeSeriesLength; t++) {
        // Extract window [t - windowMonths, t)
        std::map<std::string, std::vector<double>> windowData;
        for (const auto& [ind, fullSeries] : surprises) {
            std::vector<double> window(
                fullSeries.begin() + (t - windowMonths),
                fullSeries.begin() + t
            );
            windowData[ind] = window;
        }

        // Compute covariance for this window
        CovarianceCalculator covCalc;
        CovarianceMatrix windowCov = covCalc.calculateCovarianceMatrix(windowData);

        // Decompose
        MacroFactors factorization = decomposeSurpriseCovariance(windowCov, numFactors);

        // Track label stability
        for (int k = 0; k < numFactors; k++) {
            LabelResult currentLabel = labelFactorRobustly(
                factorization.loadings.col(k),
                factorization.indicatorNames,
                previousLabel
            );

            if (!currentLabel.isStable && currentLabel.cosineScore >= 0.65) {
                std::cerr << "Warning at t=" << t << ", factor " << k << ": "
                          << currentLabel.message << std::endl;
            }

            factorization.factorLabels[k] = currentLabel.label;
            factorization.labelConfidences[k] = currentLabel.cosineScore;

            previousLabel = currentLabel;
        }

        results.push_back(factorization);
    }

    return results;
}

std::map<std::string, Eigen::VectorXd> MacroFactorModel::getEconomicArchetypes(
    const std::vector<std::string>& indicatorNames)
{
    // Create index map: indicator name -> position in vector
    std::map<std::string, int> indexMap;
    for (size_t i = 0; i < indicatorNames.size(); i++) {
        indexMap[indicatorNames[i]] = i;
    }

    int n = indicatorNames.size();
    std::map<std::string, Eigen::VectorXd> archetypes;

    // Growth archetype: GDP↑, unemployment↓, sentiment↑
    Eigen::VectorXd growth = Eigen::VectorXd::Zero(n);
    if (indexMap.count("gdp")) growth(indexMap["gdp"]) = 1.0;
    if (indexMap.count("unemployment")) growth(indexMap["unemployment"]) = -1.0;
    if (indexMap.count("consumer_sentiment")) growth(indexMap["consumer_sentiment"]) = 1.0;
    if (growth.norm() > 1e-10) {
        archetypes["Growth"] = growth.normalized();
    }

    // Inflation archetype: CPI↑, inflation↑
    Eigen::VectorXd inflation = Eigen::VectorXd::Zero(n);
    if (indexMap.count("inflation")) inflation(indexMap["inflation"]) = 1.0;
    if (indexMap.count("cpi")) inflation(indexMap["cpi"]) = 1.0;
    if (inflation.norm() > 1e-10) {
        archetypes["Inflation"] = inflation.normalized();
    }

    // Policy archetype: Fed funds↑, spreads↑
    Eigen::VectorXd policy = Eigen::VectorXd::Zero(n);
    if (indexMap.count("fed_funds")) policy(indexMap["fed_funds"]) = 1.0;
    if (indexMap.count("treasury_10y")) policy(indexMap["treasury_10y"]) = 0.5;
    if (policy.norm() > 1e-10) {
        archetypes["Policy"] = policy.normalized();
    }

    // Volatility archetype: VIX↑, MOVE↑
    Eigen::VectorXd volatility = Eigen::VectorXd::Zero(n);
    if (indexMap.count("vix")) volatility(indexMap["vix"]) = 1.0;
    if (indexMap.count("move")) volatility(indexMap["move"]) = 1.0;
    if (volatility.norm() > 1e-10) {
        archetypes["Volatility"] = volatility.normalized();
    }

    return archetypes;
}

double MacroFactorModel::cosineSimilarity(
    const Eigen::VectorXd& v1,
    const Eigen::VectorXd& v2)
{
    if (v1.size() != v2.size()) {
        throw std::invalid_argument("Vectors must have same size");
    }

    double numerator = v1.dot(v2);
    double denominator = v1.norm() * v2.norm();

    if (denominator < 1e-10) {
        return 0.0;
    }

    double similarity = numerator / denominator;

    // Clamp to [0, 1] in case of numerical issues
    return std::max(0.0, std::min(1.0, similarity));
}
