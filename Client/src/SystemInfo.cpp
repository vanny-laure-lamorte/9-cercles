#include "SystemInfo.h"
#include <unistd.h>
#include <limits.h>
#include <cstdlib>

using namespace std;
#include <iostream>

string SystemInfo::getHostName()
{
    const string hostName = "Poopy";
    cout << "> Hostname: " << hostName << "\n";
    return std::string(hostName);
}

string SystemInfo::getUserName()
{
    const char *username = getenv("USER");
    cout << "> Username: " << username << "\n";
    return username ? string(username) : "unknown";
}

string SystemInfo::getOSInfo()
{
#ifdef _WIN32
    cout << "> OS: Windows\n";
    return "Windows";
#elif __linux__
    cout << "> OS: Linux\n";
    return "Linux";
#elif __APPLE__
    cout << "> OS: macOS\n";
    return "macOS";
#else
    cout << "> OS: Unknown\n";
    return "Unknown";
#endif
}
