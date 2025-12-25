//
//  Logger.hpp
//  InvertedYieldCurveTrader
//
//  Structured JSON logging for CloudWatch
//
//  Created by Ryan Hamby on 12/25/25.
//

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <nlohmann/json.hpp>
#include <iostream>
#include <chrono>
#include <ctime>

using json = nlohmann::json;

/**
 * Structured logging for CloudWatch integration
 *
 * Usage:
 *   Logger::info("Analysis started", {
 *       {"date", "2025-12-25"},
 *       {"indicators_fetched", 8}
 *   });
 *
 * Output (JSON):
 *   {
 *     "level": "INFO",
 *     "timestamp": "2025-12-25T10:30:45Z",
 *     "message": "Analysis started",
 *     "date": "2025-12-25",
 *     "indicators_fetched": 8
 *   }
 */
class Logger {
public:
    enum Level {
        DEBUG,
        INFO,
        WARN,
        ERROR,
        CRITICAL
    };

    /**
     * Log an informational message
     */
    static void info(const std::string& message, const json& context = json::object());

    /**
     * Log a warning
     */
    static void warn(const std::string& message, const json& context = json::object());

    /**
     * Log an error with exception context
     */
    static void error(const std::string& message, const std::exception& e,
                      const json& context = json::object());

    /**
     * Log an error without exception
     */
    static void error(const std::string& message, const json& context = json::object());

    /**
     * Log a critical error
     */
    static void critical(const std::string& message, const std::exception& e,
                         const json& context = json::object());

    /**
     * Log debug information (only in development)
     */
    static void debug(const std::string& message, const json& context = json::object());

    /**
     * Set logging level
     */
    static void setLevel(Level level);

    /**
     * Get current timestamp in ISO 8601 format
     */
    static std::string getCurrentTimestamp();

private:
    static Level currentLevel_;

    /**
     * Internal logging function
     */
    static void log(Level level, const std::string& message, const json& context);

    /**
     * Level name for output
     */
    static std::string levelName(Level level);

    /**
     * Should log this level?
     */
    static bool shouldLog(Level level);
};

#endif // LOGGER_HPP
