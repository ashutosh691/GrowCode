#include "Compiler.h"

#include <iostream>

#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

// function to compile the submitted source code
bool Compiler::compile(const std::string& sourceFile, const std::string& executableFile, const std::string& errorFile) {
    
    // create a new process for the compiler
    pid_t pid = fork();

    // fork() returns a negative value if process creation fails
    if (pid < 0) {
        std::cerr << "Failed to create compiler process."<< std::endl;
        return false;
    }

    // Inside the child process
    if (pid == 0) {

        // open the file where compiler errors will be stored
        int errorFd = open(errorFile.c_str(), O_WRONLY | O_CREAT | O_TRUNC,0644);

        // Exit if the error file cannot be opened
        if (errorFd < 0) {
            _exit(1);
        }

        // Redirect standard error (stderr) to our error file
        dup2(errorFd, STDERR_FILENO);

        // Close the temporary file descriptor
        close(errorFd);

        // replace the child process with the g++ compiler process
        execlp(
            "g++",
            "g++",
            "-std=c++17",
            sourceFile.c_str(),
            "-o",
            executableFile.c_str(),
            static_cast<char*>(nullptr)
        );

        // If execlp fails and returns, force close the child process
        _exit(1);
    }

    // variable used to store the child process status
    int status = 0;

    // wait for the compiler process to finish
    if (waitpid(pid, &status, 0) < 0) {
        return false;
    }

    // Check if the compiler finished normally without crashing
    if (WIFEXITED(status)) {

        // return true only when g++ returned exit code 0
        return WEXITSTATUS(status) == 0;
    }

    // Return false if the compiler crashed or failed
    return false;
}
