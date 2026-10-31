# GrowCode
### A Multi-User Resource-Aware Code Execution and Management Platform

GrowCode is a locally hosted, multi-user coding platform with an integrated online judge. Users can browse programming problems, write solutions in supported programming languages, submit their code, and receive automated verdicts after compilation and execution against predefined test cases.

The platform integrates **Operating System (OS)** and **Database Management System (DBMS)** concepts through a C++ execution engine, concurrent submission processing, and a MySQL-backed data management system.

---

## Problem Statement & Objectives

Existing online coding platforms generally depend on internet connectivity and remote servers. GrowCode aims to provide a college-oriented, locally hosted coding platform where multiple users can access a centralized server and practice programming problems.

### Objectives

- Develop a local, multi-user coding platform.
- Implement automated code compilation and evaluation.
- Manage users, problems, test cases, submissions, execution results, and user progress using MySQL.
- Demonstrate OS concepts such as process management, scheduling, multithreading, synchronization, IPC, and resource management.
- Demonstrate DBMS concepts such as primary keys, foreign keys, normalization, SQL queries, transactions, indexes, and concurrency.
- Provide submission history and user progress tracking.
- Analyze submitted code for estimated time and space complexity.
- Display expected complexity and recommended algorithms configured for individual problems.

---

## Team Members

| S. No. | Name |
|---|---|
| 1 | Mayank Chhetri |
| 2 | Ashutosh Upreti |
| 3 | Anuj Negi |

---

## Technologies & Tools

| Component | Technology |
|---|---|
| Backend | C++ |
| Frontend | HTML, CSS, JavaScript |
| Database | MySQL |
| Database Connectivity | MySQL Connector/C++ (X DevAPI) |
| HTTP Server | cpp-httplib |
| JSON Processing | nlohmann/json |
| Compiler | g++ |
| Build System | CMake |
| Version Control | Git, GitHub |
| Development Environment | Visual Studio Code |

### Operating System APIs

- `fork()` – Process creation.
- `exec()` – Program execution.
- `wait4()` – Process monitoring and resource usage collection.
- `pipe()` – Inter-Process Communication.
- `dup2()` – File descriptor redirection.
- `setrlimit()` – Resource limit configuration.
- `kill()` – Process termination.

---

## Project Setup & Installation

### 1. Clone the Repository

```bash
git clone https://github.com/ashutosh691/GrowCode.git
cd GrowCode
```

### 2. Start MySQL

For macOS users with MySQL installed through Homebrew:

```bash
brew services start mysql
```

### 3. Log In to MySQL

```bash
mysql -u root -p
```

Enter your MySQL password when prompted.

### 4. Initialize the Database

Execute the SQL initialization script from the project root:

```sql
source database/queries.sql;
```

Ensure the SQL file exists at the specified location. If necessary, provide its absolute path.

### 5. Configure Database Connectivity

Configure the database connection parameters in the backend according to your local MySQL installation.

Required configuration:

- Host
- Port
- Username
- Password
- Database name

**Security:** Never commit database passwords, API keys, or other credentials to the repository. Use environment variables or a local configuration file excluded from version control wherever supported.

### 6. Build the Project

From the project root:

```bash
mkdir -p build
cd build
cmake ..
make -j$(sysctl -n hw.ncpu)
```

If CMake reports missing dependencies, install and configure the required libraries before rebuilding.

### 7. Run the Application

From the project root:

```bash
./build/growcode_server
```

The application will be accessible at:

**http://localhost:8080**

Open this URL in your browser to access GrowCode.

For access from other devices on the same local network, configure the server to listen on the appropriate network interface and use the host computer's LAN IP address. Ensure firewall rules permit the required connection.

---

## Major Features & Modules

### 1. User Management

- User registration.
- User login and logout.
- Session management.
- Authentication and protected routes.
- User-specific submission history.

### 2. Problem Management

- Browse programming problems.
- View problem descriptions and constraints.
- View sample inputs and expected outputs.
- Admin problem management.
- Test-case management.
- Expected time and space complexity configuration.
- Recommended algorithm configuration.

### 3. Code Submission

- Browser-based code editor.
- Programming language selection.
- Code submission through HTTP APIs.
- Submission status tracking.
- Submission history.
- User progress tracking.

### 4. Online Judge

The integrated online judge automates the evaluation of submitted programs.

**Execution workflow:**

1. Receive source code through the backend API.
2. Compile the submitted program using the configured compiler.
3. Execute the compiled program against predefined test cases.
4. Capture program output and execution information.
5. Compare actual output with expected output.
6. Generate the corresponding verdict.
7. Store execution results in the MySQL database.

#### Supported Verdicts

| Verdict | Description |
|---|---|
| `ACCEPTED` | All test cases passed. |
| `WRONG_ANSWER` | Output did not match the expected result. |
| `COMPILATION_ERROR` | Source code compilation failed. |
| `RUNTIME_ERROR` | Program terminated abnormally. |
| `TIME_LIMIT_EXCEEDED` | Program exceeded its execution time limit. |

### 5. Complexity Analysis

- Analyze accepted submissions for estimated time complexity.
- Estimate space complexity.
- Compare estimated complexity against expected complexity configured for the problem.
- Display a complexity comparison chart.
- Retrieve recommended algorithms associated with individual problems from MySQL.
- Display explanations and confidence information when returned by the analysis service.

**Note:** Complexity estimates are analytical predictions, not formal proofs of asymptotic complexity. Their availability depends on the configured analysis service and database records.

### 6. OS-Based Execution Engine

The execution engine demonstrates core Operating System concepts involved in running submitted programs.

| OS Concept | Implementation |
|---|---|
| Process Creation | `fork()` |
| Program Execution | `exec()` |
| Process Monitoring | `wait4()` |
| Inter-Process Communication | `pipe()` |
| I/O Redirection | `dup2()` |
| Resource Management | `setrlimit()` |
| Process Termination | `kill()` |

Additional capabilities:

- Execution time monitoring.
- Resource-limit enforcement.
- Process lifecycle management.
- Isolation of individual program executions.

### 7. Scheduling & Concurrency

The submission processing system incorporates concurrent execution and scheduling mechanisms.

- Job queue for submission processing.
- Scheduler for managing execution jobs.
- Worker pool for concurrent task processing.
- Worker threads.
- Mutexes for shared-resource synchronization.
- Condition variables for coordinating worker activity.
- Concurrent processing of multiple submissions.

### 8. Database Management

MySQL provides persistent storage and structured data management for the application.

#### Data Managed

- User and authentication data.
- Programming problems.
- Test cases.
- Code submissions.
- Execution results.
- User progress.
- Expected complexity and recommended algorithm information.

#### DBMS Concepts Demonstrated

- Primary keys and foreign keys.
- Referential integrity.
- Constraints.
- Database normalization.
- Indexing.
- SQL queries.
- Transactions.
- Relationships between application entities.
- Concurrent database operations.

---

## System Architecture

GrowCode follows a client-server architecture in which multiple users interact with a centralized backend.

**High-level workflow:**

```text
             Multiple Users
                   |
                   v
          Frontend (HTML/CSS/JS)
                   |
                   v
              HTTP APIs
                   |
                   v
             C++ Backend
                   |
          +--------+--------+
          |                 |
          v                 v
     Job Scheduler      MySQL Database
          |
          v
      Job Queue
          |
          v
      Worker Pool
          |
          v
   Execution Engine
          |
          v
   Compile & Execute
          |
          v
     Test Case Evaluation
          |
          v
    Verdict Generation
          |
          v
   Store Execution Results
          |
          v
       User Interface
```

---

## Current Project Status

The project includes development and integration work across the following areas:

- [x] Project structure and requirements definition.
- [x] System architecture and workflow design.
- [x] MySQL database schema design.
- [x] SQL database initialization through `queries.sql`.
- [x] C++ database connectivity.
- [x] Submission and job management.
- [x] OS-based code execution engine.
- [x] Scheduler and worker pool.
- [x] Online judge integration.
- [x] Backend API development.
- [x] Authentication and session management.
- [x] Frontend implementation.
- [x] Submission history and user progress tracking.
- [x] Complexity-analysis integration.
- [x] Expected complexity and recommended algorithm retrieval.

### Pending / Future Improvements

- UI enhancement and responsive design.
- Messaging and communication features.
- Further security hardening of code execution.
- Additional testing and integration improvements.

---

## Project Scope

GrowCode is designed as an educational project demonstrating how Operating System mechanisms and DBMS functionality can be integrated into a practical coding platform.

It provides a foundation for understanding:

- Process execution and lifecycle management.
- CPU scheduling and concurrency.
- Multithreading and synchronization.
- Inter-Process Communication.
- Resource allocation and management.
- Relational database design and normalization.
- Transaction management and indexing.
- Backend API development and client-server communication.
- Automated compilation and program evaluation.

The platform aims to provide a locally hosted environment for collaborative programming practice while demonstrating the practical application of core OS and DBMS concepts.

---

## Repository

**GitHub:** https://github.com/ashutosh691/GrowCode

---

*GrowCode — Bringing Operating Systems, Database Management, and Competitive Programming Together.*
