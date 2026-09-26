#include <iostream>

#include "database/Database.h"

// Main function starts the program
int main()
{
    Database database(
        "127.0.0.1",
        33060,
        "root",
        "Password",
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

    // Test 1: getSubmissionStatus()

    int submissionId = 58;
    int userId = 1;

    std::cout << "\nTesting getSubmissionStatus():" << std::endl;

    std::string status =
        database.getSubmissionStatus(submissionId, userId);

    if (!status.empty())
    {
        std::cout << "Submission ID: " << submissionId << std::endl;

        std::cout << "Status: " << status << std::endl;
    }
    else
    {
        std::cout << "Submission not found or does not belong to the user." << std::endl;
    }

    // Test 2: getUserSubmissions()

    std::cout << "\nTesting getUserSubmissions():" << std::endl;

    std::string submissions = database.getUserSubmissions(userId);

    std::cout << "User submissions:" << std::endl;

    std::cout << submissions << std::endl;

    // Test 3: getUserProgress()
    std::cout << "\nTesting getUserProgress():" << std::endl;

    std::string progress = database.getUserProgress(userId);

    std::cout << "User progress:" << std::endl;

    std::cout << progress << std::endl;
    return 0;
}