#ifndef DATABASE_H
#define DATABASE_H

#include <string> //used to store text values like username and password
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
};

#endif
