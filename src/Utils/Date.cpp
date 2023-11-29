//
//  Date.cpp
//  InvertedYieldCurveTrader
//
//  Created by Ryan Hamby on 11/25/23.
//

#include "Date.hpp"
#include <iostream>

std::string getDateDaysAgo(int daysAgo = 0) {
    // Get the current system time
    auto now = std::chrono::system_clock::now();

    // Subtract daysAgo from the current time
    auto timeAgo = now - std::chrono::hours(24 * daysAgo);

    // Convert the time to a time_t object
    std::time_t currentTime = std::chrono::system_clock::to_time_t(timeAgo);

    // Convert the time_t object to a struct tm object (for local time)
    std::tm* localTime = std::localtime(&currentTime);

    // Extract year, month, and day from the struct tm
    int year = localTime->tm_year + 1900; // Years since 1900
    int month = localTime->tm_mon + 1;    // Months are zero-based
    int day = localTime->tm_mday;
    
    std::string monthString = month < 10 ? "0" + std::to_string(month) : std::to_string(month);
    std::string dayString = day < 10 ? "0" + std::to_string(day) : std::to_string(day);

    return std::to_string(year) + "_" + monthString + "_" + dayString;
}
