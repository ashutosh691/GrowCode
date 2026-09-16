#include <iostream>
#include "database/Database.h"

//Main function starts the program
int main()
{
    //Create a Database object with MySQL connection details
    Database database(
        "127.0.0.1",       //MySQL server address
        33060,             //MySQL X Protocol port
        "root",             //MySQL username
        "DB password",    //MySQL password
        "DB Name"          //Database name
    );

    //Check whether the database connection is successful
    if (database.isConnected())
    {
        std::cout << "Database connection test successful." << std::endl;
    }
    else
    {
        std::cout << "Database connection test failed." << std::endl;
    }

    return 0;
}
