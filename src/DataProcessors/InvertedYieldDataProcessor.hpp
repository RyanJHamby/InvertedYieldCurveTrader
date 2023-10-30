//
//  InvertedYieldDataProcessor.hpp
//  InvertedYieldCurveTrader
//
//  Created by Ryan Hamby on 10/1/23.
//

#ifndef InvertedYieldDataProcessor_h
#define InvertedYieldDataProcessor_h

#include <stdio.h>
#include <iostream>
#include <vector>

class InvertedYieldDataProcessor {
public:
    std::vector<double> process();
    double getMean();
    double getStdDev();
    std::vector<double> getRecentValues();
    
protected:
    double mean;
    double stdDev;
    void setMean(double mean);
    void setStdDev(double stdDev);
    void setRecentValues(std::vector<double> recentValues);
    std::vector<double> recentValues;
};

#endif /* InvertedYieldDataProcessor_h */
