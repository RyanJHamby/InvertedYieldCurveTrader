//
//  main.cpp
//  InvertedYieldCurveTrader
//
//  Created by Ryan Hamby on 8/27/23.
//

#include <iostream>
#include <aws/core/Aws.h>
#include <aws/athena/AthenaClient.h>
#include <aws/athena/model/StartQueryExecutionRequest.h>
#include <aws/athena/model/GetQueryExecutionRequest.h>
#include <aws/athena/model/GetQueryResultsRequest.h>
#include <aws/core/utils/Outcome.h>
#include <aws/core/utils/StringUtils.h>
#include <aws/core/utils/logging/LogMacros.h>


int main(int argc, const char * argv[]) {
    Aws::SDKOptions options;
    Aws::InitAPI(options);

    // Initialize the Athena client with default configuration
    Aws::Athena::AthenaClient athenaClient;

    // Replace with your Athena query
    const Aws::String query = "SELECT * FROM your_table";

    Aws::Athena::Model::StartQueryExecutionRequest startRequest;
    startRequest.SetQueryString(query);
    startRequest.SetQueryExecutionContext(Aws::Athena::Model::QueryExecutionContext().WithDatabase("your_database"));

    auto startOutcome = athenaClient.StartQueryExecution(startRequest);
    if (startOutcome.IsSuccess()) {
        const Aws::String& queryExecutionId = startOutcome.GetResult().GetQueryExecutionId();

        Aws::Athena::Model::GetQueryExecutionRequest getExecutionRequest;
        getExecutionRequest.SetQueryExecutionId(queryExecutionId);

        // Wait for the query to complete (you can implement your own polling logic)
        while (true) {
            auto getExecutionOutcome = athenaClient.GetQueryExecution(getExecutionRequest);
            if (!getExecutionOutcome.IsSuccess()) {
                std::cerr << "Failed to get query execution status: " << getExecutionOutcome.GetError().GetMessage() << std::endl;
                break;
            }

            const auto& status = getExecutionOutcome.GetResult().GetQueryExecution().GetStatus().GetState();

            if (status == Aws::Athena::Model::QueryExecutionState::SUCCEEDED) {
                break;
            } else if (status == Aws::Athena::Model::QueryExecutionState::FAILED ||
                       status == Aws::Athena::Model::QueryExecutionState::CANCELLED) {
                std::cerr << "Query execution failed or was cancelled." << std::endl;
                break;
            }

            // Wait before polling again (you can adjust the wait time)
            Aws::Utils::Threading::SleepFor(std::chrono::seconds(5));
        }

        // Query has completed, you can fetch the results now
        Aws::Athena::Model::GetQueryResultsRequest getResultsRequest;
        getResultsRequest.SetQueryExecutionId(queryExecutionId);

        auto getResultsOutcome = athenaClient.GetQueryResults(getResultsRequest);
        if (getResultsOutcome.IsSuccess()) {
            // Process and display query results
            const auto& resultRows = getResultsOutcome.GetResult().GetResultSet().GetRows();
            for (const auto& row : resultRows) {
                for (const auto& col : row.GetData()) {
                    std::cout << Aws::Utils::StringUtils::ToStdString(col.GetVarCharValue()) << "\t";
                }
                std::cout << std::endl;
            }
        } else {
            std::cerr << "Failed to get query results: " << getResultsOutcome.GetError().GetMessage() << std::endl;
        }
    } else {
        std::cerr << "Failed to start query execution: " << startOutcome.GetError().GetMessage() << std::endl;
    }

    Aws::ShutdownAPI(options);
    return 0;
}
