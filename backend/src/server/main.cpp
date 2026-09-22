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
        "Ashutosh280705",     //MySQL password
        "growcode_app"       //Database name
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

   //Test createSubmission() function
   std::cout << "\nTesting createSubmission():" << std::endl;

   int submissionId = database.createSubmission(
       1,                          //user ID
       2,                          //problem ID
       1,                          //language ID
       "#include <iostream>\n"
       "using namespace std;\n"
       "int main() {\n"
       "    int a, b;\n"
       "    cin >> a >> b;\n"
       "    cout << a + b;\n"
       "    return 0;\n"
       "}"                           //source code
   );

   //Check whether submission was created successfully
   if (submissionId != -1)
   {
       std::cout << "Submission created successfully." << std::endl;
       std::cout << "Submission ID: " << submissionId << std::endl;
   }
   else
   {
       std::cout << "Submission creation failed." << std::endl;
   }

   return 0;
}
