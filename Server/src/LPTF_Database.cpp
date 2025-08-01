#include "LPTF_Database.h"
#include <iostream>
#include <pqxx/pqxx>

LPTF_Database::LPTF_Database()
    : conn(nullptr) {
}

LPTF_Database::~LPTF_Database() {
}

bool LPTF_Database::connect() {
    try {
        cout << "[DB] Attempting to connect..." << endl;
        conn = make_unique<pqxx::connection>(
            "host=127.0.0.1 port=5432 dbname=circle user=postgres password=Lazlo"
        );

        if (!conn->is_open()) {
            cerr << "[DB] Connection object is not open." << endl;
            conn.reset();
            return false;
        }

        cout << "[DB] Connected to database successfully." << endl;
        return true;
    } catch (const exception &e) {
        cerr << "[DB] Exception on connect: " << e.what() << endl;
        return false;
    }
}

int LPTF_Database::generateUserReference() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(1000, 9999);
    return dist(gen);
}

bool LPTF_Database::insertHostInfo(int clientFd,
    const string &hostname,
    const string &username,
    const string &os,
    const string &langue,
    int userReference)
{
    if (!conn || !conn->is_open()) {
        cerr << "[DB] No open connection to DB." << endl;
        return false;
    }

    try {
        pqxx::work txn(*conn);

        txn.exec_params(
            "INSERT INTO host_info (client_fd, hostname, username, os, langue, user_reference) VALUES ($1, $2, $3, $4, $5, $6)",
            clientFd, hostname, username, os, langue, userReference
        );

        txn.commit();

        cout << "[DB] Host info inserted for clientFd = " << clientFd << endl;
        return true;
    }
    catch (const exception &e) {
        cerr << "[DB] Exception: " << e.what() << endl;
        return false;
    }
}