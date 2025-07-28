#ifndef SYSTEM_COMMAND_H
#define SYSTEM_COMMAND_H

#include <string>

/**
 * SystemCommand
 * Provides utility methods to interact with the system through shell commands.
 * Includes operations related to the user's desktop, such as listing files, creating
 * and opening text files.
 */
class SystemCommand
{
public:
    // Constructor and Destructor
    SystemCommand();
    ~SystemCommand();

    // Disable copy constructor and assignment operator
    SystemCommand(const SystemCommand &) = delete;
    SystemCommand &operator=(const SystemCommand &) = delete;

    // Returns the current user's desktop path.
    static std::string getDesktopPath();

    // Lists all files and directories on the user's desktop.
    static std::string listDesktopFiles();

    // Creates a text file on the desktop and writes the given content to it.
    static bool createTextFileOnDesktop(const std::string &filename, const std::string &content);

    // Opens a file from the desktop using the default associated application.
    static bool openFileOnDesktop(const std::string &filename);

    // Creates a text file on the desktop and opens it with Notepad.
    static std::string createAndOpenTextFileOnDesktop(const std::string &filename, const std::string &content);

    // Opens Google Chrome with the specified URL.
    bool openURLInChrome(const std::string &url);
};

#endif // SYSTEM_COMMAND_H