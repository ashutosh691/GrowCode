
#include "RunCodeExecutor.h"
#include "../sandbox/SandboxManager.h"

#include <string>
#include <exception>

RunCodeResult RunCodeExecutor::execute(
    const std::string& code,
    int languageId,
    const std::string& input
) {
    RunCodeResult result{
        "ERROR",
        "",
        "",
        0.0,
        0
    };

    if (code.empty()) {
        result.error = "Source code cannot be empty.";
        return result;
    }

    if (languageId < 1 || languageId > 3) {
        result.error = "Unsupported programming language.";
        return result;
    }

    try {
        SandboxRequest request;
        request.code = code;
        request.languageId = languageId;
        request.input = input;
        request.config = SandboxConfig{};

        SandboxResult sandboxResult = SandboxManager::execute(request);

        result.status = sandboxResult.status;
        result.output = sandboxResult.output;
        result.error = sandboxResult.error;
        result.executionTime = sandboxResult.executionTime;
        result.memoryUsed = sandboxResult.memoryUsed;

        if (sandboxResult.status == "COMPILATION_ERROR" &&
            result.error.empty()) {
            result.error = "Compilation failed.";
        }

        if (sandboxResult.status == "RUNTIME_ERROR" &&
            result.error.empty()) {
            result.error =
                "Program terminated with exit code " +
                std::to_string(sandboxResult.exitCode) + ".";
        }

        if (sandboxResult.status == "TIME_LIMIT_EXCEEDED" &&
            result.error.empty()) {
            result.error = "Time limit exceeded.";
        }
    }
    catch (const std::exception& error) {
        result.status = "ERROR";
        result.error = error.what();
    }

    return result;
}
