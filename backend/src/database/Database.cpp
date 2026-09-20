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
