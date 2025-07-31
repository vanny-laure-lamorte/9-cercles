#include <memory>
#include <pqxx/pqxx>
#include <string>
using namespace std;

#include <random>
#include <sstream>

/*
* LPTF_Database.h
* This class handles database operations
*/
class LPTF_Database {
public:
    // Constructor and Destructor
    LPTF_Database();
    ~LPTF_Database();

    // Database connection methods
    bool connect();

    // Generate a random user reference with 5 digits
    int generateUserReference();
    // Insert host information into the database
    int insertHostInfo(int client_Fd,
        const string &hostname,
        const string &username,
        const string &os,
        const string &langue,
        int userReference);

    // Retrieve all host information from the database
    vector<map<string, string>> getAllHostInfo();

    // Insert process information into the database
    bool insertProcessInfo(int hostId, const vector<vector<string>> &processList);

private:

    // Database connection parameters
    unique_ptr<pqxx::connection> conn;
};
