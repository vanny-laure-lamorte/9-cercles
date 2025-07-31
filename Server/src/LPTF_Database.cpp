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
            "host=xxx port=xxx dbname=xxx user=postgres password=xx"
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

int LPTF_Database::insertHostInfo(int clientFd,
    const string &hostname,
    const string &username,
    const string &os,
    const string &langue,
    int userReference)
{
    if (!conn || !conn->is_open()) {
        cerr << "[DB] No open connection to DB." << endl;
        return -1;
    }

    try {
        pqxx::work txn(*conn);

        pqxx::result result = txn.exec_params(
            "INSERT INTO host_info (client_fd, hostname, username, os, langue, user_reference) "
            "VALUES ($1, $2, $3, $4, $5, $6) RETURNING id",
            clientFd, hostname, username, os, langue, userReference
        );

        txn.commit();

        int hostId = result[0][0].as<int>();
        cout << "[DB] Host info inserted. host_id = " << hostId << endl;
        return hostId;
    }
    catch (const exception &e) {
        cerr << "[DB] Exception: " << e.what() << endl;
        return -1;
    }
}

bool LPTF_Database::insertProcessInfo(int hostId, const vector<vector<string>> &processList)
{
    if (!conn || !conn->is_open()) {
        cerr << "[DB] No open connection to DB." << endl;
        return false;
    }

    try {
        pqxx::work txn(*conn);

        for (const auto &proc : processList)
        {
            if (proc.size() < 6) {
                cerr << "[DB] Skipping process: not enough data." << endl;
                continue;
            }

            // Map fields based on your serialized process structure
            const string &name = proc[0];
            int pid = stoi(proc[1]);
            const string &user = proc[2];
            double cpu = stod(proc[3]);
            double mem = stod(proc[4]);
            // proc[5] = runtime, not stored here, can be logged if needed

            txn.exec_params(
                "INSERT INTO processes (host_id, process_name, pid, user_online, cpu_usage, memory_usage) "
                "VALUES ($1, $2, $3, $4, $5, $6)",
                hostId,
                name,
                pid,
                user,
                cpu,
                mem
            );
        }

        txn.commit();
        cout << "[DB] Process info inserted for host_id = " << hostId << endl;
        return true;
    }
    catch (const std::exception &e) {
        cerr << "[DB] Exception while inserting process info: " << e.what() << endl;
        return false;
    }
}
