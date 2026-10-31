#ifndef RUN_CODE_EXECUTOR_H
#define RUN_CODE_EXECUTOR_H

#include <string>

struct RunCodeResult {
    std::string status;
    std::string output;
    std::string error;
    double executionTime;
    long long memoryUsed;
};

class RunCodeExecutor {
public:
    static RunCodeResult execute(
        const std::string& code,
        int languageId,
        const std::string& input
    );
};

#endif