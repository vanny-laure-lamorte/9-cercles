#include "../include/ProcessManager.h"

vector<vector<string>> ProcessManager::getRunningProcesses()
{
    vector<vector<string>> processCombined;

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE)
    {
        return processCombined;
    }

    PROCESSENTRY32 processEntry;
    processEntry.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(snapshot, &processEntry))
    {
        do
        {
            vector<string> processInfo;
            string exeName = wcharToString(processEntry.szExeFile);
            processInfo.push_back(exeName);
            processInfo.push_back(to_string(processEntry.th32ProcessID));
            processInfo.push_back(to_string(processEntry.th32ParentProcessID));
            processInfo.push_back(to_string(processEntry.cntThreads));
            processInfo.push_back(to_string(processEntry.pcPriClassBase));
            processInfo.push_back(getProcessRuntime(processEntry.th32ProcessID));
            processCombined.push_back(processInfo);
        } while (Process32Next(snapshot, &processEntry));
    }

    CloseHandle(snapshot);

    return processCombined;
}

string ProcessManager::getProcessRuntime(DWORD pid)
{
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
    if (hProcess == NULL)
    {
        return "N/A";
    }

    FILETIME creationTime, exitTime, kernelTime, userTime;
    if (!GetProcessTimes(hProcess, &creationTime, &exitTime, &kernelTime, &userTime))
    {
        CloseHandle(hProcess);
        return "N/A";
    }
    CloseHandle(hProcess);

    FILETIME nowFileTime;
    GetSystemTimeAsFileTime(&nowFileTime);

    ULONGLONG creation = (((ULONGLONG)creationTime.dwHighDateTime) << 32) | creationTime.dwLowDateTime;
    ULONGLONG now = (((ULONGLONG)nowFileTime.dwHighDateTime) << 32) | nowFileTime.dwLowDateTime;

    ULONGLONG uptime = now - creation;
    ULONGLONG seconds = uptime / 10000000;

    ostringstream oss;
    oss << (seconds / 3600) << "h "
        << ((seconds % 3600) / 60) << "m "
        << (seconds % 60) << "s";

    return oss.str();
}

string ProcessManager::wcharToString(const wchar_t *wstr)
{
    if (!wstr)
        return "";

    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, nullptr, 0, nullptr, nullptr);
    string result(size_needed - 1, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr, -1, &result[0], size_needed, nullptr, nullptr);

    return result;
}