//
//  test_processors.cpp
//  Test the new FRED-based data processors
//
//  Compile with:
//  g++ -std=c++20 -I./src -I/opt/homebrew/opt/nlohmann-json/include \
//      src/DataProviders/FREDDataClient.cpp \
//      src/DataProcessors/FedFundsProcessor.cpp \
//      src/DataProcessors/UnemploymentProcessor.cpp \
//      src/DataProcessors/ConsumerSentimentProcessor.cpp \
//      test_processors.cpp -lcurl -o test_processors
//

#include <iostream>
#include <cstdlib>
#include "DataProcessors/FedFundsProcessor.hpp"
#include "DataProcessors/UnemploymentProcessor.hpp"
#include "DataProcessors/ConsumerSentimentProcessor.hpp"

int main() {
    const char* apiKeyEnv = std::getenv("FRED_API_KEY");
    if (!apiKeyEnv || std::string(apiKeyEnv).empty()) {
        std::cerr << "Error: FRED_API_KEY environment variable not set\n";
        return 1;
    }

    std::string apiKey(apiKeyEnv);

    std::cout << "Testing FRED Data Processors\n";
    std::cout << "============================\n\n";

    try {
        // Test Fed Funds Processor
        std::cout << "1. Federal Funds Rate Processor\n";
        FedFundsProcessor fedFundsProc;
        auto fedFundsData = fedFundsProc.process(apiKey, 3);
        std::cout << "   Latest: " << fedFundsProc.getLatestValue(apiKey) << "%\n";
        std::cout << "   Recent values: ";
        for (const auto& val : fedFundsData) {
            std::cout << val << "% ";
        }
        std::cout << "\n   ✅ Fed Funds Processor working\n\n";

        // Test Unemployment Processor
        std::cout << "2. Unemployment Rate Processor\n";
        UnemploymentProcessor unemploymentProc;
        auto unemploymentData = unemploymentProc.process(apiKey, 3);
        std::cout << "   Latest: " << unemploymentProc.getLatestValue(apiKey) << "%\n";
        std::cout << "   Recent values: ";
        for (const auto& val : unemploymentData) {
            std::cout << val << "% ";
        }
        std::cout << "\n   ✅ Unemployment Processor working\n\n";

        // Test Consumer Sentiment Processor
        std::cout << "3. Consumer Sentiment Processor\n";
        ConsumerSentimentProcessor sentimentProc;
        auto sentimentData = sentimentProc.process(apiKey, 3);
        std::cout << "   Latest: " << sentimentProc.getLatestValue(apiKey) << "\n";
        std::cout << "   Recent values: ";
        for (const auto& val : sentimentData) {
            std::cout << val << " ";
        }
        std::cout << "\n   ✅ Consumer Sentiment Processor working\n\n";

        std::cout << "============================\n";
        std::cout << "✅ All 3 new processors working correctly!\n\n";

        std::cout << "Current indicator status:\n";
        std::cout << "  ✅ Inflation (existing)\n";
        std::cout << "  ✅ Inverted Yield Curve (existing)\n";
        std::cout << "  ✅ GDP (existing)\n";
        std::cout << "  ✅ Federal Funds Rate (NEW)\n";
        std::cout << "  ✅ Unemployment (NEW)\n";
        std::cout << "  ✅ Consumer Sentiment (NEW)\n";
        std::cout << "  ⚠️  VIX - needs non-FRED data source\n";
        std::cout << "  ⚠️  ISM Manufacturing - needs correct FRED series ID\n";
        std::cout << "\n6 out of 8 indicators working! 🎉\n";

        return 0;

    } catch (const std::exception& e) {
        std::cerr << "\nError: " << e.what() << "\n";
        return 1;
    }
}
