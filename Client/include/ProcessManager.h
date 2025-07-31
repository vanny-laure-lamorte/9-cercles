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
 * ProcessManager
 * Get a list of running processes on the system.
 */
class ProcessManager
{
public:
    // Returns a vector of vectors containing process information
    static vector<vector<string>> getRunningProcesses();

private:
    // Returns the runtime of a process in a human-readable format
    static string getProcessRuntime(DWORD pid);

    // Converts a wide character string to a standard string
    static string wcharToString(const wchar_t *wstr);

};

#endif // PROCESS_MANAGER_H