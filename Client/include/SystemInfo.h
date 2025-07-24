#ifndef SYSTEM_INFO_H
#define SYSTEM_INFO_H
using namespace std;
#include <string>

/*
 * SystemInfo
 * This class provides methods to retrieve system information such as hostname, username, and OS information.
 */
class SystemInfo {

public:
    // Constructor and destructor
    SystemInfo() = default;
    ~SystemInfo() = default;

    // Disable copy constructor and assignment operator
    SystemInfo(const SystemInfo&) = delete;
    // Disable assignment operator
    SystemInfo& operator=(const SystemInfo&) = delete;

    // Get all system information as a formatted string
    static string getAllInfo();
    // Get hostname of the system
    static string getHostName();
    // Get username of the system
    static string getUserName();
    // Get operating system information
    static string getOSInfo();
    // Get the language of the operating system
    static string getOSLanguage();

private:
    const static string DEFAULT_HOSTNAME;
    const static string DEFAULT_LOGIN;
    const static string DEFAULT_OS_INFO;
    const static string DEFAULT_OS_LANGUAGE;
};

#endif