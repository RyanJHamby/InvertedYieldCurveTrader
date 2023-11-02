//
//  StatsCalculator.hpp
//  InvertedYieldCurveTrader
//
//  Created by Ryan Hamby on 9/30/23.
//

#ifndef StatsCalculator_hpp
#define StatsCalculator_hpp

#include <vector>
#include <tuple>

class StatsCalculator {
public:
    std::vector<double> getData();
    double calculateMean(const std::vector<double>& data);
    void setData(const std::string& jsonData);
    void setStockData(const std::string& jsonData);
    double calculateCovarianceConfidence(const double averageCovariance);

protected:
    std::vector<double> data;
};

#endif /* StatsCalculator_hpp */
