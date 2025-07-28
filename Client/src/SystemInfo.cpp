#include "SystemInfo.h"

#include <unistd.h>
#include <limits.h>
#include <cstdlib>
#include <winsock2.h>
#include <windows.h>
#include <locale>
#include <codecvt>
#include <iostream>

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 255
#endif
#pragma comment(lib, "Ws2_32.lib")

using namespace std;

string SystemInfo::getAllInfo()
{
        string hostname = SystemInfo::getHostName();
        string username = SystemInfo::getUserName();
        string os = SystemInfo::getOSInfo();
        string osLanguage = SystemInfo::getOSLanguage();
        string allInfo = ">> Hostname: " + hostname + "\n" + ">> Username: " + username + "\n" + ">> OS: " + os + "\n" + ">> OS Language: " + osLanguage + "\n";

        return allInfo;
}

string SystemInfo::getHostName()
{
    char hostName[HOST_NAME_MAX];
    if (gethostname(hostName, sizeof(hostName)) == 0) {
        return string(hostName);
    } else {
        perror("gethostname");
        return "unknown";
    }
}

string SystemInfo::getUserName()
{
    const char* username = getenv("USER");
    if (!username)
        username = getenv("USERNAME");
    if (username)
        return std::string(username);
    else
        return "unknown";
}

string SystemInfo::getOSInfo()
{
#ifdef _WIN32
    return "Windows";
#elif __linux__
    return "Linux";
#elif __APPLE__
    return "macOS";
#else
    return "Unknown";
#endif
}

string SystemInfo::getOSLanguage()
{
#ifdef _WIN32
    LANGID langId = GetUserDefaultLangID();
    wchar_t langName[LOCALE_NAME_MAX_LENGTH];
    if (LCIDToLocaleName(langId, langName, LOCALE_NAME_MAX_LENGTH, 0) > 0) {
        return std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(langName);
    } else {
        return "unknown";
    }
#elif __linux__ || __APPLE__
    const char* lang = getenv("LANG");
    if (lang) {
        return string(lang);
    } else {
        return "unknown";
    }
#else
    return "unknown";
#endif
}