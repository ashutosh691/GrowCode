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
        std::cout << "Database connection test successful." << std::endl;
    }
    else
    {
        std::cout << "Database connection test failed." << std::endl;
        return 1;
    }

    // Test 1: Get all queued submission IDs
    std::cout << "\nTesting getQueuedSubmissionIds():" << std::endl;

    std::vector<int> queuedSubmissionIds = database.getQueuedSubmissionIds();

    if (queuedSubmissionIds.empty())
    {
        std::cout << "No queued submissions found." << std::endl;
        return 0;
    }

    std::cout << "Queued Submission IDs:" << std::endl;

    for (int id : queuedSubmissionIds)
    {
        std::cout << id << std::endl;
    }

    // Use the first queued submission for the next tests
    int submissionId = queuedSubmissionIds[0];

    std::cout << "\nUsing Submission ID: "
              << submissionId << std::endl;

    // Test 2: Get submission code
    std::cout << "\nTesting getSubmissionCode():" << std::endl;

    std::string code =
        database.getSubmissionCode(submissionId);

    if (!code.empty())
    {
        std::cout << "Submitted Code:" << std::endl;
        std::cout << code << std::endl;
    }
    else
    {
        std::cout << "Submission code not found." << std::endl;
    }

    // Test 3: Get problem ID
    std::cout << "\nTesting getSubmissionProblemId():" << std::endl;

    int problemId =
        database.getSubmissionProblemId(submissionId);

    if (problemId != -1)
    {
        std::cout << "Problem ID: " << problemId << std::endl;
    }
    else
    {
        std::cout << "Problem ID not found." << std::endl;
    }

    // Test 4: Get language ID
    std::cout << "\nTesting getSubmissionLanguageId():" << std::endl;

    int languageId = database.getSubmissionLanguageId(submissionId);

    if (languageId != -1)
    {
        std::cout << "Language ID: " << languageId << std::endl;
    }
    else
    {
        std::cout << "Language ID not found." << std::endl;
    }

    return 0;
}