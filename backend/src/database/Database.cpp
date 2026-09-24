#include"database/Database.h"  //Includes the Database class declaration
#include<iostream> //Used for printing messages
#include<mysqlx/xdevapi.h> //MySQL library used to connect to the database

//Constructor of the Database class which receives all the information needed to connect to MySQL

Database::Database(
    const std::string& host, //MySQL server address 
    int port,  //MySQL port number
    const std::string& username, //MySQL username
    const std::string& password, //MySQL password
    const std::string& database //Name of the database
)
   //Initialize the class variables with the given values
    : host(host), port(port), username(username), password(password), database(database)
{
    //try block contains the code that may cause a connection error
    try {
        //Create a MySQL session using the given login details
        mysqlx::Session session(
            host, //Server address
            port,  //Port number
            username, password 
        );
   //Select the database that we want to use
        session.sql("USE " + database).execute();
   //Display a success message if the connection works
        std::cout << "Connected to MySQL database: " << database << std::endl;
    }
    //catch block handles MySQL connection errors
    catch (const mysqlx::Error& e) {
        // Display the error message
        std::cerr << "Database connection failed: " << e.what() << std::endl;
//Send the error back to the program
        throw;
    }
}

//Checks whether the database connection is successful
bool Database::isConnected()
{
    try {
        //Create a MySQL session using the given login details
        mysqlx::Session session(host, port, username, password);

        //Select the database that we want to use
        session.sql("USE " + database).execute();

        //Run a simple query to check whether the connection works
        session.sql("SELECT 1").execute();

        return true;
    }
    catch (...)
    {
        return false;
    }
}

// Retreives all the problems from the Database
std::vector<Problem> Database::getProblems() {
    std::vector<Problem> problems; // vector to be returned

    try {
        mysqlx::Session session( host, port, username, password);
        session.sql("USE " + database).execute();

        // executing db query to get all details of a problem
        auto result = session.sql(
            "SELECT problem_id, title, description, difficulty, constraints "
            "FROM problems "
            "ORDER BY problem_id"
        ).execute();

        // pushing retrieved values to a tuple 
        for (auto row : result) {
            Problem problem;
            problem.problem_id = row[0].get<int>();
            problem.title = row[1].get<std::string>();
            problem.description = row[2].get<std::string>();
            problem.difficulty = row[3].get<std::string>();
            problem.constraints = row[4].get<std::string>();
            problems.push_back(problem);
        }
    }
    catch (const mysqlx::Error& e) {
        std::cerr << "Failed to get problems: " << e.what() << std::endl;
    }
    return problems;
}

// Retrieves problem by its problem ID
Problem Database::getProblem(int problem_id) {
    mysqlx::Session session( host, port, username, password);
    session.sql("USE " + database).execute();

    // db query for retrieving problem by its problem id
    auto result = session.sql(
        "SELECT problem_id, title, description, difficulty, constraints "
        "FROM problems "
        "WHERE problem_id = ?"
    ).bind(problem_id).execute();

    auto row = result.fetchOne();

    if (row.isNull()) {
        throw std::runtime_error("Problem not found");
    }

    Problem problem;
    //store the values to a tuple if found
    problem.problem_id = row[0].get<int>();
    problem.title = row[1].get<std::string>();
    problem.description = row[2].get<std::string>();
    problem.difficulty = row[3].get<std::string>();
    problem.constraints = row[4].get<std::string>();

    return problem;
}

std::vector<TestCase> Database::getTestCases(int problemId) {
    std::vector<TestCase> testCases;
    try {
        mysqlx::Session session(host, port, username, password);
        session.sql("USE " + database).execute();
        // db query for retrieving all test cases for a problem id
        auto result = session.sql(
            "SELECT test_case_id, input, expected_output, is_sample "
            "FROM test_cases "
            "WHERE problem_id = ? "
            "ORDER BY order_no"
        ).bind(problemId).execute();

        // storing retrieved info to a tuple
        for (auto row : result.fetchAll()) {
            TestCase testCase;

            testCase.testCaseId = row[0].get<int>();

            if (row[1].isNull()) {
                testCase.input = "";
            } else {
                testCase.input = row[1].get<std::string>();
            }

            testCase.expectedOutput = row[2].get<std::string>();
            testCase.isSample = row[3].get<bool>();
            testCases.push_back(testCase);
        }
    }
    catch (const mysqlx::Error& e) {
        std::cerr << "Get test cases error: " << e.what() << std::endl;
    }
    return testCases;
}


// // function to create submission for user 
// int Database::createSubmission(int userId, int problemId, int languageId, const std::string& code) {
//     try {
//         mysqlx::Session session( host, port, username, password);
//         session.sql("USE " + database).execute();

//         // query to save the submitted code of the user in the database
//         auto result = session.sql(
//             "INSERT INTO submissions "
//             "(user_id, problem_id, language_id, code, status) "
//             "VALUES (?, ?, ?, ?, 'PENDING')"
//         )
//         .bind(userId)
//         .bind(problemId)
//         .bind(languageId)
//         .bind(code)
//         .execute(); // binding received values respectively

//         return static_cast<int>(result.getAutoIncrementValue()); // returns the auto generated submission id
//     }
//     catch (const mysqlx::Error& e) {
//         std::cerr << "Submission creation failed: " << e.what() << std::endl;
//         return -1;
//     }
// }

// // function to create a job for a submission
// bool Database::createJob(int submissionId) {
//     try {
//         // create a session to connect with the database
//         mysqlx::Session session(
//             host,
//             port,
//             username,
//             password
//         );

//         // select the database to work with
//         session.sql("USE " + database).execute();

//         // query to create a job for the submitted code
//         // the job is initially placed in QUEUED status
//         // priority is set to 0 by default
//         session.sql(
//             "INSERT INTO jobs "
//             "(submission_id, status, priority) "
//             "VALUES (?, 'QUEUED', 0)"
//         )
//         .bind(submissionId) // bind the submission ID to the query
//         .execute();

//         // return true if the job was created successfully
//         return true;
//     }
//     catch (const mysqlx::Error& e) {
//         // display the error if job creation fails
//         std::cerr
//             << "Job creation failed: "
//             << e.what()
//             << std::endl;

//         // return false when job creation fails
//         return false;
//     }
// }

// combined above two functions for transaction handling
// function to create a submission and its job together
int Database::createSubmissionWithJob(int userId, int problemId, int languageId, const std::string& code) {
    try {
        // create a session to connect with the database
        mysqlx::Session session(host, port, username, password);

        // select the database to work with
        session.sql("USE " + database).execute();

        // start a transaction so submission and job are treated as one operation
        session.sql("START TRANSACTION").execute();

        try {
            // query to insert the submitted code into the submissions table
            auto submissionResult = session.sql(
                "INSERT INTO submissions "
                "(user_id, problem_id, language_id, code, status) "
                "VALUES (?, ?, ?, ?, 'PENDING')"
            )
            .bind(userId, problemId, languageId, code).execute();

            // get the auto-generated submission ID
            int submissionId = static_cast<int>(submissionResult.getAutoIncrementValue());

            // query to create a job for the newly created submission
            // the job is initially placed in QUEUED status
            // priority is set to 0
            session.sql(
                "INSERT INTO jobs "
                "(submission_id, status, priority) "
                "VALUES (?, 'QUEUED', 0)"
            )
            .bind(submissionId) // bind the generated submission ID
            .execute();

            // save both operations permanently
            session.sql("COMMIT").execute();

            // return the generated submission ID
            return submissionId;
        }
        catch (...) {
            // undo all database changes if any operation fails
            session.sql("ROLLBACK").execute();  // TRANSACTION handling

            // pass the error to the outer catch block
            throw;
        }
    }
    catch (const mysqlx::Error& e) {
        // display the error if submission or job creation fails
        std::cerr
            << "Submission and job creation failed: " << e.what() << std::endl;

        // return -1 to indicate failure
        return -1;
    }
}