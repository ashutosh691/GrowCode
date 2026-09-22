#include <iostream>
#include "database/Database.h"

//Main function starts the program
int main()
{
    //Create a Database object with MySQL connection details
    Database database(
        "127.0.0.1",        //MySQL server address
        33060,               //MySQL X Protocol port
        "root",              //MySQL username
        "Pass",     //MySQL password
        "DB Name"       //Database name
    );

   //Check whether the database connection is successful
   if (database.isConnected())
   {
       std::cout << "Database connection test successful." << std::endl;
   }
   else
   {
       std::cout << "Database connection test failed." << std::endl;
       return 1;
   }


   //Test getProblems() function
   std::cout << "\nTesting getProblems():" << std::endl;

   std::vector<Problem> problems = database.getProblems();

   for (const auto& problem : problems)
   {
       std::cout << "Problem ID: " << problem.problem_id << std::endl;
       std::cout << "Title: " << problem.title << std::endl;
       std::cout << "Difficulty: " << problem.difficulty << std::endl;
       std::cout << "Constraints: " << problem.constraints << std::endl;
       std::cout << std::endl;
   }


   //Test getProblem() function
   std::cout << "\nTesting getProblem(2):" << std::endl;

   Problem problem = database.getProblem(2);

   std::cout << "Problem ID: " << problem.problem_id << std::endl;
   std::cout << "Title: " << problem.title << std::endl;
   std::cout << "Description: " << problem.description << std::endl;
   std::cout << "Difficulty: " << problem.difficulty << std::endl;
   std::cout << "Constraints: " << problem.constraints << std::endl;


   //Test getProblem() with an invalid problem ID
   std::cout << "\nTesting getProblem(999):" << std::endl;

   try
   {
       Problem problem = database.getProblem(999);

       std::cout << "Problem ID: " << problem.problem_id << std::endl;
       std::cout << "Title: " << problem.title << std::endl;
   }
   catch (const std::exception& e)
   {
       std::cout << "Error: " << e.what() << std::endl;
   }


   //Test getTestCases() function
   std::cout << "\nTesting getTestCases(2):" << std::endl;

   std::vector<TestCase> testCases = database.getTestCases(2);

   for (const auto& testCase : testCases)
   {
       std::cout << "Test Case ID: " << testCase.testCaseId << std::endl;
       std::cout << "Input: " << testCase.input << std::endl;
       std::cout << "Expected Output: " << testCase.expectedOutput << std::endl;
       std::cout << "Sample: " << testCase.isSample << std::endl;
       std::cout << std::endl;
   }


   //Test getTestCases() with an invalid problem ID
   std::cout << "\nTesting getTestCases(999):" << std::endl;

   std::vector<TestCase> invalidTestCases = database.getTestCases(999);

   std::cout << "Number of test cases: " << invalidTestCases.size() << std::endl;
   return 0;
}
