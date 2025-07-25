#include "../include/ProcessManager.h"

string ProcessManager::getRunningProcesses()
{
    std::ostringstream processCombined;

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE)
    {
        return "";
    }

    PROCESSENTRY32 processEntry;
    processEntry.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(snapshot, &processEntry))
    {
        do
        {
            processCombined << processEntry.szExeFile << "\n";
        } while (Process32Next(snapshot, &processEntry));
    }

    CloseHandle(snapshot);
    return processCombined.str();
}