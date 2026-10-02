#include <iostream>
#include <thread>
#include <chrono>

#include "database/Database.h"

#include "execution-engine/scheduler/Scheduler.h"
#include "execution-engine/executor/ExecutionWorker.h"
#include "execution-engine/worker/WorkerPool.h"

// Main function starts the execution engine test
int main()
{
    // Create database connection
    Database database(
        "127.0.0.1",
        33060,
        "root",
        "Pass",
        "growcode_app"
    );

    // Check database connection
    if (!database.isConnected())
    {
        std::cout
            << "Database connection test failed."
            << std::endl;

        return 1;
    }

    std::cout
        << "Database connection successful."
        << std::endl;

    // Create a test submission
    //
    // Problem 2 is "Sum of Two Numbers".
    //
    // Its test cases are:
    // 5 7       -> 12
    // 10 20     -> 30
    // 100 250   -> 350

    std::string code = R"(
#include <iostream>

int main()
{
    int a, b;

    std::cin >> a >> b;

    std::cout << a - b;

    return 0;
}
)";

    // Create submission and job together.
    //
    // userId = 1
    // problemId = 2
    // languageId = 1

    int submissionId =
        database.createSubmissionWithJob(
            1,
            2,
            1,
            code
        );

    if (submissionId == -1)
    {
        std::cout
            << "Failed to create submission."
            << std::endl;

        return 1;
    }

    std::cout
        << "Created Submission ID: "
        << submissionId
        << std::endl;

    // Create Scheduler
    Scheduler scheduler;

    // Create ExecutionWorker
    ExecutionWorker executionWorker(database);

    // Create WorkerPool with two worker threads
    WorkerPool workerPool(
        scheduler,
        executionWorker,
        2
    );

    // Add submission to the scheduler queue
    scheduler.addJob(submissionId);

    std::cout
        << "Submission added to scheduler."
        << std::endl;

    // Start worker threads
    workerPool.start();

    std::cout
        << "Worker pool started."
        << std::endl;

    // Give the worker time to process the submission.
    //
    // This is only for testing.
    // The actual server will continuously run.

    std::this_thread::sleep_for(
        std::chrono::seconds(5)
    );

    // Stop worker pool
    workerPool.stop();

    std::cout
        << "Worker pool stopped."
        << std::endl;

    // Check final submission status
    std::string status =
        database.getSubmissionStatus(
            submissionId,
            1
        );

    std::cout
        << "\nFinal Submission Status: "
        << status
        << std::endl;

    // Check whether the complete execution engine worked
    if (status == "ACCEPTED")
    {
        std::cout
            << "\nExecution engine test successful."
            << std::endl;
    }
    else
    {
        std::cout
            << "\nExecution engine test failed."
            << std::endl;
    }

    return 0;
}