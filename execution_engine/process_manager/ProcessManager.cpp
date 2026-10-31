
#include "ProcessManager.h"
#include "../sandbox/SandboxManager.h"

#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <fcntl.h>
#include <signal.h>
#include <chrono>
#include <thread>
#include <cerrno>
#include <algorithm>
#include <climits>

// Preserve compatibility with the existing executable interface.
int ProcessManager::runProcess(
    const std::string& executable,
    const std::string& inputFile,
    const std::string& outputFile,
    const std::string& workingDirectory,
    long long& memoryUsed,
    const std::string& errorFile
) {
    return runProcess(
        std::vector<std::string>{executable},
        inputFile,
        outputFile,
        workingDirectory,
        memoryUsed,
        errorFile,
        SandboxConfig{}
    );
}

// Preserve compatibility with existing vector-based calls.
int ProcessManager::runProcess(
    const std::vector<std::string>& command,
    const std::string& inputFile,
    const std::string& outputFile,
    const std::string& workingDirectory,
    long long& memoryUsed,
    const std::string& errorFile
) {
    return runProcess(
        command,
        inputFile,
        outputFile,
        workingDirectory,
        memoryUsed,
        errorFile,
        SandboxConfig{}
    );
}

// Execute a program using the supplied sandbox configuration.
int ProcessManager::runProcess(
    const std::vector<std::string>& command,
    const std::string& inputFile,
    const std::string& outputFile,
    const std::string& workingDirectory,
    long long& memoryUsed,
    const std::string& errorFile,
    const SandboxConfig& config
) {
    memoryUsed = 0;

    if (command.empty() || command[0].empty()) {
        return -1;
    }

    int pipeFd[2];

    if (pipe(pipeFd) != 0) {
        return -1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        close(pipeFd[0]);
        close(pipeFd[1]);
        return -1;
    }

    if (pid == 0) {
        close(pipeFd[0]);

        // Enforce CPU time limit.
        struct rlimit cpuLimit {};

        cpuLimit.rlim_cur = config.cpuTimeLimitSeconds;
        cpuLimit.rlim_max = config.cpuTimeLimitSeconds;

        if (setrlimit(RLIMIT_CPU, &cpuLimit) != 0) {
            _exit(1);
        }

        // Apply an address-space limit where supported.
#ifdef RLIMIT_AS
        if (config.memoryLimitBytes > 0) {
            struct rlimit memoryLimit {};

            memoryLimit.rlim_cur =
                static_cast<rlim_t>(config.memoryLimitBytes);

            memoryLimit.rlim_max =
                static_cast<rlim_t>(config.memoryLimitBytes);

            setrlimit(RLIMIT_AS, &memoryLimit);
        }
#endif

        if (chdir(workingDirectory.c_str()) != 0) {
            _exit(1);
        }

        const char message[] = "EXECUTION_STARTED";

        write(pipeFd[1], message, sizeof(message));
        close(pipeFd[1]);

        int inputFd = open(inputFile.c_str(), O_RDONLY);

        int outputFd = open(
            outputFile.c_str(),
            O_WRONLY | O_CREAT | O_TRUNC,
            0644
        );

        int errorFd = -1;

        if (!errorFile.empty()) {
            errorFd = open(
                errorFile.c_str(),
                O_WRONLY | O_CREAT | O_TRUNC,
                0644
            );
        }

        if (inputFd < 0 || outputFd < 0 ||
            (!errorFile.empty() && errorFd < 0)) {

            if (inputFd >= 0) close(inputFd);
            if (outputFd >= 0) close(outputFd);
            if (errorFd >= 0) close(errorFd);

            _exit(1);
        }

        if (dup2(inputFd, STDIN_FILENO) < 0 ||
            dup2(outputFd, STDOUT_FILENO) < 0) {

            close(inputFd);
            close(outputFd);

            if (errorFd >= 0) close(errorFd);

            _exit(1);
        }

        if (errorFd >= 0 &&
            dup2(errorFd, STDERR_FILENO) < 0) {

            close(inputFd);
            close(outputFd);
            close(errorFd);

            _exit(1);
        }

        close(inputFd);
        close(outputFd);

        if (errorFd >= 0) {
            close(errorFd);
        }

        std::vector<char*> args;

        args.reserve(command.size() + 1);

        for (const std::string& arg : command) {
            args.push_back(const_cast<char*>(arg.c_str()));
        }

        args.push_back(nullptr);

        execvp(args[0], args.data());

        _exit(127);
    }

    close(pipeFd[1]);

    char message[64] = {};

    ssize_t bytesRead = read(
        pipeFd[0],
        message,
        sizeof(message) - 1
    );

    close(pipeFd[0]);

    if (bytesRead > 0) {
        message[bytesRead] = '\0';
    }

    std::cout << "IPC message: " << message << std::endl;

    int status = 0;
    struct rusage usage {};

    const double timeLimit =
        config.wallTimeLimitSeconds > 0
            ? config.wallTimeLimitSeconds
            : 2.0;

    auto startTime = std::chrono::steady_clock::now();

    while (true) {
        pid_t result = wait4(
            pid,
            &status,
            WNOHANG,
            &usage
        );

        if (result == pid) {
            memoryUsed = static_cast<long long>(
                usage.ru_maxrss
            );

            if (WIFEXITED(status)) {
                return WEXITSTATUS(status);
            }

            if (WIFSIGNALED(status)) {
                return 128 + WTERMSIG(status);
            }

            return -1;
        }

        if (result < 0) {
            if (errno == EINTR) {
                continue;
            }

            return -1;
        }

        auto currentTime = std::chrono::steady_clock::now();

        double elapsed = std::chrono::duration<double>(
            currentTime - startTime
        ).count();

        if (elapsed >= timeLimit) {
            kill(pid, SIGKILL);

            while (wait4(pid, &status, 0, &usage) < 0) {
                if (errno != EINTR) {
                    return -1;
                }
            }

            memoryUsed = static_cast<long long>(
                usage.ru_maxrss
            );

            return -2;
        }

        std::this_thread::sleep_for(
            std::chrono::milliseconds(10)
        );
    }
}
