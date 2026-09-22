# GrowCode

GrowCode is a locally hosted, multi-user coding platform with an integrated online judge. Users can browse programming problems, write C++ solutions, submit their code, and receive automated verdicts after compilation and execution against predefined test cases.

## Problem Statement / Objective

Existing online coding platforms generally depend on Internet connectivity and remote servers. GrowCode aims to provide a college-oriented, locally hosted coding platform where multiple users can access a central server and practice programming problems.

The main objectives are:

- Develop a local multi-user coding platform.
- Implement automated C++ compilation and code evaluation.
- Manage users, problems, test cases, submissions, and results using MySQL.
- Demonstrate Operating System concepts such as process management, scheduling, multithreading, synchronization, IPC, and resource management.
- Demonstrate DBMS concepts such as primary keys, foreign keys, normalization, SQL queries, transactions, indexes, and concurrency.

## Team Members

- Mayank Chhetri
- Ashutosh Upreti
- Anuj Negi

## Technologies / Tools Used

- **Backend:** C++
- **Frontend:** HTML, CSS, JavaScript
- **Database:** MySQL
- **Database Connectivity:** MySQL Connector/C++ (X DevAPI)
- **HTTP Server:** cpp-httplib
- **JSON:** nlohmann/json
- **Compiler:** g++
- **OS APIs:** fork(), exec(), wait4(), pipe(), dup2(), setrlimit(), kill()
- **Build System:** CMake
- **Version Control:** Git and GitHub
- **Development Environment:** Visual Studio Code

## Project Setup / Installation Instructions

### 1. Clone the Repository

git clone https://github.com/ashutosh691/GrowCode.git
cd GrowCode

### 2. Start the MySQL Service

```bash
brew services start mysql
```

### 3. Login to MySQL

```bash
mysql -u root -p
```

### 4. Create the Database and Tables

Run the SQL file:

```sql
source database/queries.sql
```

### 5. Configure Database Connection

Configure the MySQL connection details in the backend using your local database credentials.

> **Note:** Do not commit the actual database password to the repository.

### 6. Build the Project

From the project root:

```bash
rm -rf build
mkdir build
cd build
cmake ..
make -j$(sysctl -n hw.ncpu)
```

### 7. Run the Project

Return to the project root:

```bash
cd ..
./build/growcode_server
```

The server will be available at:

```text
http://localhost:8080
```

---

## Major Features / Modules

### User Management

- User registration
- User login and logout
- Session management
- Authentication and protected routes

### Problem Management

- Browse programming problems
- View problem details
- Admin problem management
- Test-case management

### Code Submission

- C++ code editor
- Code submission through HTTP API
- Submission history
- User progress tracking

### Online Judge

- C++ compilation using `g++`
- Execution against predefined test cases
- Output comparison
- Automated verdict generation

### Supported Verdicts

- `ACCEPTED`
- `WRONG_ANSWER`
- `COMPILATION_ERROR`
- `RUNTIME_ERROR`
- `TIME_LIMIT_EXCEEDED`

### OS Execution Engine

- Process creation using `fork()`
- Program execution using `exec()`
- Process monitoring using `wait4()`
- Inter-Process Communication using `pipe()`
- File descriptor management using `dup2()`
- Resource and time management
- Process termination using `kill()`

### Scheduling and Concurrency

- Job queue
- Scheduler
- Worker threads
- Mutex and condition variables
- Concurrent execution of multiple submissions

### Database Management

- User and authentication data
- Problems and test cases
- Submissions and execution results
- User progress
- Primary and foreign keys
- Constraints and indexes
- SQL operations and transactions

---

## Current Project Status / Progress

- Project structure created
- Requirements and system architecture defined
- Database schema designed
- `queries.sql` created
- MySQL database and tables created
- C++ database connection

### Pending
- Backend API development
- Authentication implementation
- Frontend implementation
- Submission and job management
- OS execution engine
- Scheduler and worker pool
- Online judge integration
- Testing and integration
- LAN multi-user deployment
