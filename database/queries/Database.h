#ifndef DATABASE_H
#define DATABASE_H

#include <string>

class Database {
private:
    std::string host;
    int port;
    std::string username;
    std::string password;
    std::string database;

public:
    Database(
        const std::string& host,
        int port,
        const std::string& username,
        const std::string& password,
        const std::string& database
    );

    bool isConnected();
};

#endif
