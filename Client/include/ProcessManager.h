#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

using namespace std;
#pragma once
#include <vector>
#include <string>

/*
* Get a list of running processes on the system.
*/
class ProcessManager {
public:
    static vector<string> getRunningProcesses();
};

#endif
