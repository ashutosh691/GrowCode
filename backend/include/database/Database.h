#ifndef DATABASE_H
#define DATABASE_H

#include "problems/Problem.h"
#include "problems/TestCase.h"
#include <string> 
#include<vector>
//database class is used to store database connection details
class Database {
private:
    std::string host; //stores database server address
    int port; //stores port number of database
    std::string username; //stores database username
    std::string password; //stores database password
    std::string database; //stores database name

//this constructor is used to initialize all database details
public:
    Database(
        const std::string& host,// Database server address
        int port,// Database port number
        const std::string& username, // Database username
        const std::string& password, // Database password
        const std::string& database  // Database name
    );
    // Checks whether the database connection is successful and returns true if connected, otherwise false
    
    bool isConnected();

    // functions to retrieve problems and test cases from the database
    std::vector<Problem> getProblems();
    Problem getProblem(int problem_id);
    std::vector<TestCase> getTestCases(int problemId);
    
    // func to create a new submission by a user
    int createSubmission(int userId, int problemId, int languageId, const std::string& code);

    //function for job creation i.e. submit
    bool createJob(int submissionId);

    // function to create a submission and its job together
    int createSubmissionWithJob(int userId, int problemId, int languageId, const std::string& code);

    // func to retrieve submitted query
    std::vector<int> getQueuedSubmissionIds();

    std::string getSubmissionCode(int submissionId);

    int getSubmissionProblemId(int submissionId);

    int getSubmissionLanguageId(int submissionId);
};

#endif
