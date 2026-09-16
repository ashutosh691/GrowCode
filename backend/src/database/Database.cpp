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
    : host(host),
      port(port),
      username(username),
      password(password),
      database(database)
{
    //try block contains the code that may cause a connection error
    try {
        //Create a MySQL session using the given login details
        mysqlx::Session session(
            host, //Server address
            port,  //Port number
            username,
            password 
        );
   //Select the database that we want to use
        session.sql("USE " + database).execute();
   //Display a success message if the connection works
        std::cout
            << "Connected to MySQL database: "
            << database
            << std::endl;
    }
    //catch block handles MySQL connection errors
    catch (const mysqlx::Error& e) {
        // Display the error message
        std::cerr
            << "Database connection failed: "
            << e.what()
            << std::endl;
//Send the error back to the program
        throw;
    }
}


