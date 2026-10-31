#include "ExecutionWorker.h"
#include "../sandbox/SandboxManager.h"

#include <iostream>
#include <string>
#include <vector>

static std::string normalizeOutput(const std::string& output) {
    std::string normalized = output;

    while (!normalized.empty() &&
           (normalized.back() == ' ' ||
            normalized.back() == '\t' ||
            normalized.back() == '\n' ||
            normalized.back() == '\r')) {
        normalized.pop_back();
    }

    return normalized;
}

ExecutionWorker::ExecutionWorker(Database& database)
    : database(database) {
}

void ExecutionWorker::processJob(int submissionId) {
    std::cout << "Worker processing Submission "
              << submissionId << std::endl;

    database.updateSubmissionError(submissionId, "");
    database.updateSubmissionStatus(submissionId, "RUNNING");
    database.updateJobStatus(submissionId, "RUNNING");

    int problemId = database.getSubmissionProblemId(submissionId);
    std::string code = database.getSubmissionCode(submissionId);
    int languageId = database.getSubmissionLanguageId(submissionId);

    if (languageId < 1 || languageId > 3) {
        database.updateSubmissionStatus(
            submissionId, "COMPILATION_ERROR"
        );

        database.updateUserProgress(
            submissionId, "COMPILATION_ERROR"
        );

        database.updateJobStatus(submissionId, "FAILED");

        std::cout << "Submission " << submissionId
                  << ": Unsupported language ID "
                  << languageId << std::endl;
        return;
    }

    if (code.empty()) {
        database.updateSubmissionStatus(
            submissionId, "COMPILATION_ERROR"
        );

        database.updateUserProgress(
            submissionId, "COMPILATION_ERROR"
        );

        database.updateJobStatus(submissionId, "FAILED");
        return;
    }

    std::vector<TestCase> testCases =
        database.getTestCases(problemId);

    if (testCases.empty()) {
        database.updateSubmissionStatus(
            submissionId, "RUNTIME_ERROR"
        );

        database.updateJobStatus(submissionId, "FAILED");

        database.updateUserProgress(
            submissionId, "RUNTIME_ERROR"
        );

        std::cout << "Submission " << submissionId
                  << ": No test cases found" << std::endl;
        return;
    }

    bool allAccepted = true;
    std::string finalStatus = "ACCEPTED";

    for (const TestCase& testCase : testCases) {
        SandboxRequest request;
        request.code = code;
        request.languageId = languageId;
        request.input = testCase.input;
        request.config = SandboxConfig{};

        SandboxResult sandboxResult =
            SandboxManager::execute(request);

        double timeTaken = sandboxResult.executionTime;
        long long memoryUsed = sandboxResult.memoryUsed;

        std::string actualOutput = sandboxResult.output;
        std::string runtimeErrorMessage = sandboxResult.error;

        if (sandboxResult.status == "COMPILATION_ERROR") {
            if (runtimeErrorMessage.empty()) {
                runtimeErrorMessage = "Compilation failed.";
            }

            database.updateSubmissionError(
                submissionId, runtimeErrorMessage
            );

            database.updateSubmissionStatus(
                submissionId, "COMPILATION_ERROR"
            );

            database.updateUserProgress(
                submissionId, "COMPILATION_ERROR"
            );

            database.updateJobStatus(submissionId, "FAILED");

            std::cout << "Submission " << submissionId
                      << ": Compilation Error" << std::endl;
            return;
        }

        std::string normalizedActual =
            normalizeOutput(actualOutput);

        std::string normalizedExpected =
            normalizeOutput(testCase.expectedOutput);

        std::string resultStatus;

        if (sandboxResult.status == "TIME_LIMIT_EXCEEDED") {
            resultStatus = "TIME_LIMIT_EXCEEDED";
            finalStatus = resultStatus;
            allAccepted = false;

            database.updateSubmissionError(
                submissionId,
                "Time limit exceeded."
            );
        }
        else if (sandboxResult.status == "RUNTIME_ERROR" ||
                 sandboxResult.status == "ERROR") {
            resultStatus = "RUNTIME_ERROR";
            finalStatus = resultStatus;
            allAccepted = false;

            if (runtimeErrorMessage.empty()) {
                runtimeErrorMessage = "Program execution failed.";

                if (sandboxResult.exitCode >= 0) {
                    runtimeErrorMessage +=
                        " Exit code: " +
                        std::to_string(sandboxResult.exitCode) + ".";
                }
            }

            database.updateSubmissionError(
                submissionId, runtimeErrorMessage
            );
        }
        else if (normalizedActual != normalizedExpected) {
            resultStatus = "WRONG_ANSWER";
            finalStatus = resultStatus;
            allAccepted = false;
        }
        else {
            resultStatus = "ACCEPTED";
        }

        database.saveExecutionResult(
            submissionId,
            testCase.testCaseId,
            resultStatus,
            actualOutput,
            timeTaken,
            memoryUsed
        );

        std::cout << "Submission " << submissionId
                  << " | Test Case " << testCase.testCaseId
                  << " | " << resultStatus
                  << " | Time: " << timeTaken
                  << " seconds" << std::endl;

        if (!allAccepted) {
            break;
        }
    }

    if (allAccepted) {
        database.updateSubmissionStatus(
            submissionId, "ACCEPTED"
        );

        database.updateUserProgress(
            submissionId, "ACCEPTED"
        );

        database.updateJobStatus(submissionId, "COMPLETED");

        std::cout << "Submission " << submissionId
                  << ": ACCEPTED" << std::endl;
    }
    else {
        database.updateSubmissionStatus(
            submissionId, finalStatus
        );

        database.updateUserProgress(
            submissionId, finalStatus
        );

        database.updateJobStatus(submissionId, "COMPLETED");

        std::cout << "Submission " << submissionId
                  << ": " << finalStatus << std::endl;
    }
}
