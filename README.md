GrowCode

GrowCode is a locally hosted, multi-user coding platform with an integrated online judge. Users can browse programming problems, write solutions in supported programming languages, submit their code, and receive automated verdicts after compilation and execution against predefined test cases.

The platform combines Operating System and Database Management System concepts through a C++ execution engine, concurrent submission processing, and a MySQL-backed data management system.

Problem Statement / Objective

Existing online coding platforms generally depend on Internet connectivity and remote servers. GrowCode aims to provide a college-oriented, locally hosted coding platform where multiple users can access a central server and practice programming problems.

The main objectives are:

* Develop a local, multi-user coding platform.
* Implement automated code compilation and evaluation.
* Manage users, problems, test cases, submissions, execution results, and user progress using MySQL.
* Demonstrate Operating System concepts such as process management, scheduling, multithreading, synchronization, inter-process communication (IPC), and resource management.
* Demonstrate DBMS concepts such as primary keys, foreign keys, normalization, SQL queries, transactions, indexes, and concurrency.
* Provide submission history and user progress tracking.
* Analyze submitted code for estimated time and space complexity.
* Display expected complexity and recommended algorithms configured for individual problems.

Team Members

* Mayank Chhetri
* Ashutosh Upreti
* Anuj Negi

Technologies / Tools Used

* Backend: C++
* Frontend: HTML, CSS, JavaScript
* Database: MySQL
* Database Connectivity: MySQL Connector/C++ (X DevAPI)
* HTTP Server: cpp-httplib
* JSON Processing: nlohmann/json
* Compiler: g++
* OS APIs: fork(), exec(), wait4(), pipe(), dup2(), setrlimit(), kill()
* Build System: CMake
* Version Control: Git and GitHub
* Development Environment: Visual Studio Code

Project Setup / Installation Instructions

1. Clone the Repository

git clone https://github.com/ashutosh691/GrowCode.git
cd GrowCode

2. Start the MySQL Service

On macOS with MySQL installed through Homebrew, run:

brew services start mysql

3. Log In to MySQL

mysql -u root -p

Enter your MySQL password when prompted.

4. Create the Database and Tables

From the MySQL shell, execute the project’s SQL initialization file:

source database/queries.sql

Ensure the SQL file exists at the specified path relative to your current working directory. If necessary, use the absolute path to queries.sql.

5. Configure the Database Connection

Configure the MySQL connection details in the backend according to your local environment.

Ensure that the configured values match your MySQL installation:

* Host
* Port
* Username
* Password
* Database name

Security note: Do not commit database passwords, API keys, or other credentials to the repository. Use environment variables or a local configuration file excluded from version control where supported.

6. Build the Project

From the project root, run:

mkdir -p build
cd build
cmake ..
make -j$(sysctl -n hw.ncpu)

If CMake reports missing dependencies, install and configure the required libraries before rebuilding.

7. Run the Project

From the project root, start the compiled server:

./build/growcode_server

The local server will be available at:

http://localhost:8080

Open this address in your browser to access the application.

For access from other devices on the same local network, configure the server to listen on the appropriate network interface and use the host computer’s LAN IP address. Ensure that firewall rules permit the required connection.

⸻

Major Features / Modules

1. User Management

* User registration
* User login and logout
* Session management
* Authentication and protected routes
* User-specific submission history

2. Problem Management

* Browse programming problems
* View problem descriptions and constraints
* View sample inputs and expected outputs
* Admin problem management
* Test-case management
* Expected time and space complexity configuration
* Recommended algorithm configuration

3. Code Submission

* Browser-based code editor
* Language selection
* Code submission through HTTP APIs
* Submission status tracking
* Submission history
* User progress tracking

4. Online Judge

* Source-code compilation using the configured compiler
* Execution against predefined test cases
* Comparison of actual and expected output
* Automated verdict generation
* Reporting of compilation and runtime errors
* Execution time and resource-limit monitoring

Supported Verdicts

* ACCEPTED
* WRONG_ANSWER
* COMPILATION_ERROR
* RUNTIME_ERROR
* TIME_LIMIT_EXCEEDED

5. Complexity Analysis

* Analyze accepted submissions for estimated time complexity.
* Estimate space complexity.
* Compare estimated complexity against expected complexity configured for the problem.
* Display a complexity comparison chart.
* Retrieve the recommended algorithm associated with a problem from the MySQL database.
* Display an explanation and confidence information when returned by the analysis service.

Note: Complexity estimates are analytical predictions, not formal proofs of asymptotic complexity. Their availability depends on the configured analysis service and database records.

6. OS Execution Engine

The execution engine demonstrates Operating System concepts involved in running submitted programs.

* Process creation using fork()
* Program execution using exec()
* Process monitoring using wait4()
* Inter-Process Communication using pipe()
* File descriptor redirection using dup2()
* Resource limits using setrlimit()
* Process termination using kill()
* Execution time and resource management

7. Scheduling and Concurrency

* Job queue for submission processing
* Scheduler for managing execution jobs
* Worker pool for concurrent task processing
* Worker threads
* Mutexes for shared-resource synchronization
* Condition variables for coordinating worker activity
* Concurrent processing of multiple submissions

8. Database Management

MySQL stores and manages the application’s persistent data, including:

* User and authentication data
* Programming problems
* Test cases
* Code submissions
* Execution results
* User progress
* Expected complexity and recommended algorithm information

The database design demonstrates:

* Primary and foreign keys
* Referential integrity
* Constraints
* Normalization
* Indexes
* SQL queries
* Transactions
* Relationships between application entities

⸻

Current Project Status / Progress

The project includes the following development areas:

* Project structure and requirements definition
* System architecture and workflow design
* MySQL database schema design
* SQL database initialization through queries.sql
* C++ database connectivity
* Submission and job management
* OS-based code execution engine
* Scheduler and worker pool
* Online judge integration
* Backend API development
* Authentication and session management
* Frontend implementation
* Submission history and user progress tracking
* Complexity-analysis integration
* Expected complexity and recommended algorithm retrieval
* Testing and integration

Pending / Future Improvements

* UI enhancement and responsive design
* Add Messaging features
* Further security hardening of code execution

⸻

Project Scope

GrowCode is designed as an educational project that demonstrates how Operating System mechanisms and DBMS functionality can be integrated into a practical coding platform.

It provides a foundation for understanding process execution, concurrency, resource management, database design, API communication, and automated program evaluation in a locally hosted environment.