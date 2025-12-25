//
//  Logger.cpp
//  InvertedYieldCurveTrader
//
//  Created by Ryan Hamby on 12/25/25.
//

#include "Logger.hpp"
#include <iomanip>
#include <sstream>

// Static member initialization
Logger::Level Logger::currentLevel_ = Logger::INFO;

void Logger::info(const std::string& message, const json& context) {
    log(INFO, message, context);
}

void Logger::warn(const std::string& message, const json& context) {
    log(WARN, message, context);
}

void Logger::error(const std::string& message, const std::exception& e, const json& context) {
    json errorContext = context;
    errorContext["exception"] = e.what();
    log(ERROR, message, errorContext);
}

void Logger::error(const std::string& message, const json& context) {
    log(ERROR, message, context);
}

void Logger::critical(const std::string& message, const std::exception& e, const json& context) {
    json criticalContext = context;
    criticalContext["exception"] = e.what();
    log(CRITICAL, message, criticalContext);
}

void Logger::debug(const std::string& message, const json& context) {
    log(DEBUG, message, context);
}

void Logger::setLevel(Level level) {
    currentLevel_ = level;
}

std::string Logger::getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;

    std::stringstream ss;
    ss << std::put_time(std::gmtime(&time), "%Y-%m-%dT%H:%M:%S");
    ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    ss << 'Z';

    return ss.str();
}

void Logger::log(Level level, const std::string& message, const json& context) {
    if (!shouldLog(level)) {
        return;
    }

    // Build JSON log entry
    json logEntry;
    logEntry["level"] = levelName(level);
    logEntry["timestamp"] = getCurrentTimestamp();
    logEntry["message"] = message;

    // Merge context
    for (auto& [key, value] : context.items()) {
        logEntry[key] = value;
    }

    // Output to stdout (CloudWatch will capture this)
    std::cout << logEntry.dump() << std::endl;
    std::cout.flush();
}

std::string Logger::levelName(Level level) {
    switch (level) {
        case DEBUG: return "DEBUG";
        case INFO: return "INFO";
        case WARN: return "WARN";
        case ERROR: return "ERROR";
        case CRITICAL: return "CRITICAL";
        default: return "UNKNOWN";
    }
}

bool Logger::shouldLog(Level level) {
    return level >= currentLevel_;
}
