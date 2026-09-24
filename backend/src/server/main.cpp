#include <iostream>
#include <vector>

#include "database/Database.h"

// Main function starts the program
int main()
{
    Database database(
        "127.0.0.1",
        33060,
        "root",
        "YOUR_PASSWORD",
        "growcode_app"
    );

    // Check database connection
    if (database.isConnected())
    {
        std::cout
            << "Database connection test successful."
            << std::endl;
    }
    else
    {
        std::cout
            << "Database connection test failed."
            << std::endl;

        return 1;
    }

    // Get queued submissions
    std::vector<int> queuedSubmissionIds =
        database.getQueuedSubmissionIds();

    if (queuedSubmissionIds.empty())
    {
        std::cout
            << "\nNo queued submissions found."
            << std::endl;

        return 0;
    }

    // Use the first queued submission
    int submissionId = queuedSubmissionIds[0];

    std::cout
        << "\nUsing Submission ID: "
        << submissionId
        << std::endl;

    // Test 1: updateSubmissionStatus()

    std::cout << "\nTesting updateSubmissionStatus():" << std::endl;

    bool submissionUpdated =
        database.updateSubmissionStatus(
            submissionId,
            "RUNNING"
        );

    if (submissionUpdated)
    {
        std::cout
            << "Submission status updated successfully."
            << std::endl;
    }
    else
    {
        std::cout
            << "Failed to update submission status."
            << std::endl;
    }

    // Test 2: updateJobStatus()

    std::cout << "\nTesting updateJobStatus():" << std::endl;

    bool jobUpdated =
        database.updateJobStatus(
            submissionId,
            "RUNNING"
        );

    if (jobUpdated)
    {
        std::cout
            << "Job status updated successfully."
            << std::endl;
    }
    else
    {
        std::cout
            << "Failed to update job status."
            << std::endl;
    }

    // Get problem ID for the submission

    int problemId =
        database.getSubmissionProblemId(submissionId);

    if (problemId == -1)
    {
        std::cout
            << "Could not find problem ID."
            << std::endl;

        return 1;
    }

    std::cout
        << "\nProblem ID: "
        << problemId
        << std::endl;

    // Get test cases for the problem

    std::vector<TestCase> testCases =
        database.getTestCases(problemId);

    if (testCases.empty())
    {
        std::cout
            << "No test cases found for this problem."
            << std::endl;

        return 1;
    }

    int testCaseId = testCases[0].testCaseId;

    std::cout
        << "Using Test Case ID: "
        << testCaseId
        << std::endl;

    // Test 3: saveExecutionResult()

    std::cout << "\nTesting saveExecutionResult():" << std::endl;

    bool resultSaved =
        database.saveExecutionResult(
            submissionId,
            testCaseId,
            "ACCEPTED",
            "12",
            15.5,
            1024
        );

    if (resultSaved)
    {
        std::cout
            << "Execution result saved successfully."
            << std::endl;
    }
    else
    {
        std::cout
            << "Failed to save execution result."
            << std::endl;
    }

    return 0;
}
