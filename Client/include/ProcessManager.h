#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

using namespace std;
#pragma once
#include <windows.h>
#include <tlhelp32.h>
#include <vector>
#include <string>
#include <sstream>


/*
* Get a list of running processes on the system.
*/
class ProcessManager {
public:
    static string getRunningProcesses();
};

#endif
