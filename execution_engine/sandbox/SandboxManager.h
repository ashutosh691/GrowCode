
#ifndef SANDBOX_MANAGER_H
#define SANDBOX_MANAGER_H

#include <string>

struct SandboxConfig {
    int cpuTimeLimitSeconds = 2;
    int wallTimeLimitSeconds = 2;
    long long memoryLimitBytes = 256LL * 1024 * 1024;
    int processLimit = 10;
};

struct SandboxRequest {
    std::string code;
    int languageId;
    std::string input;
    SandboxConfig config;
};

struct SandboxResult {
    std::string status;
    std::string output;
    std::string error;

    int exitCode = -1;
    long long memoryUsed = 0;
    double executionTime = 0.0;

    bool timedOut = false;
    bool compilationFailed = false;
};

class SandboxManager {
public:
    static SandboxResult execute(
        const SandboxRequest& request
    );
};

#endif
