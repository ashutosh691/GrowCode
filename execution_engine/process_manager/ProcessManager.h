
#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include <string>
#include <vector>

struct SandboxConfig;

class ProcessManager {
public:
    static int runProcess(
        const std::string& executable,
        const std::string& inputFile,
        const std::string& outputFile,
        const std::string& workingDirectory,
        long long& memoryUsed,
        const std::string& errorFile = ""
    );

    static int runProcess(
        const std::vector<std::string>& command,
        const std::string& inputFile,
        const std::string& outputFile,
        const std::string& workingDirectory,
        long long& memoryUsed,
        const std::string& errorFile = ""
    );

    static int runProcess(
        const std::vector<std::string>& command,
        const std::string& inputFile,
        const std::string& outputFile,
        const std::string& workingDirectory,
        long long& memoryUsed,
        const std::string& errorFile,
        const SandboxConfig& config
    );
};

#endif
