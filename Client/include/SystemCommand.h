#ifndef SYSTEM_COMMAND_H
#define SYSTEM_COMMAND_H

#include <string>

/**
 * @brief Provides utility methods to interact with the system through shell commands.
 * Includes operations related to the user's desktop, such as listing files, creating
 * and opening text files.
 */
class SystemCommand
{
public:
    SystemCommand();
    ~SystemCommand();

    // Disable copy constructor and assignment operator
    SystemCommand(const SystemCommand &) = delete;
    SystemCommand &operator=(const SystemCommand &) = delete;

    /**
     * @brief Returns the current user's desktop path.
     * @return The full absolute path to the desktop.
     */
    static std::string getDesktopPath();

    /**
     * @brief Lists all files and directories on the user's desktop.
     * @return A string containing the output of the system 'dir' command.
     */
    static std::string listDesktopFiles();

    /**
     * @brief Creates a text file on the desktop and writes the given content to it.
     * @param filename Name of the file to create (e.g. "file.txt").
     * @param content The text content to write inside the file.
     * @return True if the file was successfully created and written, false otherwise.
     */
    static bool createTextFileOnDesktop(const std::string &filename, const std::string &content);

    /**
     * @brief Opens a file from the desktop using the default associated application.
     * @param filename Name of the file to open.
     * @return True if the system was able to launch the file, false otherwise.
     */
    static bool openFileOnDesktop(const std::string &filename);

    /**
     * @brief Creates a text file on the desktop and opens it with Notepad.
     * @param filename Name of the file to create and open.
     * @param content The content to write inside the file.
     * @return A status message indicating success or error.
     */
    static std::string createAndOpenTextFileOnDesktop(const std::string &filename, const std::string &content);

    /**
     * @brief Opens Google Chrome with the specified URL.
     *
     * @param url The URL to open in Chrome.
     * @return true if the command was executed successfully, false otherwise.
     */
    bool openURLInChrome(const std::string &url);
};

#endif // SYSTEM_COMMAND_H
