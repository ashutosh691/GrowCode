
#include "SandboxManager.h"

#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <cstdio>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>

namespace fs = std::filesystem;

namespace {

constexpr const char* DOCKER_IMAGE = "growcode-sandbox:latest";

std::string readFile(const std::string& path) {
    std::ifstream file(path);

    if (!file.is_open()) {
        return "";
    }

    return std::string(
        std::istreambuf_iterator<char>(file),
        std::istreambuf_iterator<char>()
    );
}

bool writeFile(
    const std::string& path,
    const std::string& content
) {
    std::ofstream file(path);

    if (!file.is_open()) {
        return false;
    }

    file << content;
    return file.good();
}

int runCommand(
    const std::vector<std::string>& command,
    int timeoutSeconds,
    bool& timedOut
) {
    timedOut = false;

    pid_t pid = fork();

    if (pid < 0) {
        return -1;
    }

    if (pid == 0) {
        std::vector<char*> args;

        for (const std::string& argument : command) {
            args.push_back(const_cast<char*>(argument.c_str()));
        }

        args.push_back(nullptr);

        execvp(args[0], args.data());
        _exit(127);
    }

    auto start = std::chrono::steady_clock::now();

    int status = 0;

    while (true) {
        pid_t result = waitpid(pid, &status, WNOHANG);

        if (result == pid) {
            if (WIFEXITED(status)) {
                return WEXITSTATUS(status);
            }

            if (WIFSIGNALED(status)) {
                return 128 + WTERMSIG(status);
            }

            return -1;
        }

        if (result < 0) {
            return -1;
        }

        auto elapsed = std::chrono::duration_cast<
            std::chrono::seconds
        >(
            std::chrono::steady_clock::now() - start
        ).count();

        if (elapsed >= timeoutSeconds) {
            timedOut = true;
            kill(pid, SIGKILL);
            waitpid(pid, &status, 0);
            return -1;
        }

        std::this_thread::sleep_for(
            std::chrono::milliseconds(10)
        );
    }
}

void killContainer(const std::string& containerName) {
    pid_t pid = fork();

    if (pid == 0) {
        execlp(
            "docker",
            "docker",
            "kill",
            containerName.c_str(),
            static_cast<char*>(nullptr)
        );

        _exit(127);
    }

    if (pid > 0) {
        waitpid(pid, nullptr, 0);
    }
}

std::string createExecutionScript(int languageId) {
    std::string script;

    script += "set +e\n";

    if (languageId == 1) {
        script +=
            "g++ -std=c++17 -O2 -pipe Main.cpp -o main "
            "2> compile_error.txt\n"
            "if [ $? -ne 0 ]; then\n"
            "  cp compile_error.txt error.txt\n"
            "  echo COMPILATION_ERROR > status.txt\n"
            "  exit 0\n"
            "fi\n"
            "./main < input.txt > output.txt 2> error.txt\n";
    }
    else if (languageId == 2) {
        script +=
            "javac Main.java 2> compile_error.txt\n"
            "if [ $? -ne 0 ]; then\n"
            "  cp compile_error.txt error.txt\n"
            "  echo COMPILATION_ERROR > status.txt\n"
            "  exit 0\n"
            "fi\n"
            "java Main < input.txt > output.txt 2> error.txt\n";
    }
    else if (languageId == 3) {
        script +=
            "python3 Main.py < input.txt > output.txt 2> error.txt\n";
    }
    else {
        return "";
    }

    script +=
        "exit_code=$?\n"
        "echo $exit_code > exit_code.txt\n"
        "echo EXECUTION > status.txt\n"
        "exit 0\n";

    return script;
}

}

SandboxResult SandboxManager::execute(
    const SandboxRequest& request
) {
    SandboxResult result;

    std::string sourceFile;

    if (request.code.empty()) {
        result.status = "ERROR";
        result.error = "Source code cannot be empty.";
        return result;
    }

    switch (request.languageId) {
        case 1:
            sourceFile = "Main.cpp";
            break;

        case 2:
            sourceFile = "Main.java";
            break;

        case 3:
            sourceFile = "Main.py";
            break;

        default:
            result.status = "ERROR";
            result.error = "Unsupported language ID.";
            return result;
    }

    char workspaceTemplate[] = "/tmp/growcode_sandbox_XXXXXX";

    char* workspacePath = mkdtemp(workspaceTemplate);

    if (workspacePath == nullptr) {
        result.status = "ERROR";
        result.error = "Failed to create execution workspace.";
        return result;
    }

    std::string workspace(workspacePath);

    struct WorkspaceCleanup {
        std::string path;

        ~WorkspaceCleanup() {
            std::error_code error;
            fs::remove_all(path, error);
        }
    } cleanup{workspace};

    if (chmod(workspace.c_str(), 0777) != 0) {
        result.status = "ERROR";
        result.error = "Failed to configure workspace permissions.";
        return result;
    }

    std::string sourcePath = workspace + "/" + sourceFile;
    std::string inputPath = workspace + "/input.txt";
    std::string scriptPath = workspace + "/run.sh";

    if (!writeFile(sourcePath, request.code) ||
        !writeFile(inputPath, request.input)) {
        result.status = "ERROR";
        result.error = "Failed to prepare execution files.";
        return result;
    }

    std::string script = createExecutionScript(request.languageId);

    if (script.empty() || !writeFile(scriptPath, script)) {
        result.status = "ERROR";
        result.error = "Failed to prepare execution script.";
        return result;
    }

    std::string containerName =
        "growcode-" + fs::path(workspace).filename().string();

    long long memoryMB =
        (request.config.memoryLimitBytes + 1024 * 1024 - 1)
        / (1024 * 1024);

    std::vector<std::string> command = {
        "docker",
        "run",
        "--rm",
        "--name", containerName,
        "--network", "none",
        "--memory", std::to_string(memoryMB) + "m",
        "--memory-swap", std::to_string(memoryMB) + "m",
        "--cpus", "1",
        "--pids-limit", std::to_string(request.config.processLimit),
        "--read-only",
        "--tmpfs", "/tmp:rw,noexec,nosuid,size=64m",
        "--cap-drop", "ALL",
        "--security-opt", "no-new-privileges",
        "--user", "1001:1001",
        "--mount",
        "type=bind,src=" + workspace + ",dst=/workspace",
        "--workdir", "/workspace",
        "--entrypoint", "/bin/bash",
        DOCKER_IMAGE,
        "/workspace/run.sh"
    };

    auto start = std::chrono::steady_clock::now();

    bool timedOut = false;

    int dockerExitCode = runCommand(
        command,
        request.config.wallTimeLimitSeconds,
        timedOut
    );

    auto end = std::chrono::steady_clock::now();

    result.executionTime = std::chrono::duration<double>(
        end - start
    ).count();

    if (timedOut) {
        killContainer(containerName);

        result.status = "TIME_LIMIT_EXCEEDED";
        result.timedOut = true;
        result.error = "Execution exceeded the wall-clock time limit.";
        return result;
    }

    if (dockerExitCode != 0) {
        result.status = "ERROR";
        result.error = "Docker execution failed. Check that Docker Desktop is running and the sandbox image exists.";
        result.exitCode = dockerExitCode;
        return result;
    }

    result.output = readFile(workspace + "/output.txt");
    result.error = readFile(workspace + "/error.txt");

    std::string status = readFile(workspace + "/status.txt");

    if (status.find("COMPILATION_ERROR") != std::string::npos) {
        result.status = "COMPILATION_ERROR";
        result.compilationFailed = true;
        return result;
    }

    if (status.find("EXECUTION") == std::string::npos) {
        result.status = "ERROR";
        result.error = "Execution did not produce a valid result.";
        return result;
    }

    std::string exitCodeText = readFile(
        workspace + "/exit_code.txt"
    );

    try {
        result.exitCode = std::stoi(exitCodeText);
    }
    catch (...) {
        result.status = "ERROR";
        result.error = "Failed to read program exit code.";
        return result;
    }

    if (result.exitCode == 0) {
        result.status = "SUCCESS";
    }
    else {
        result.status = "RUNTIME_ERROR";
    }

    return result;
}
