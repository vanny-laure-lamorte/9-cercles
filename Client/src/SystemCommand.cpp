#include "SystemCommand.h"
#include "SystemInfo.h"

#include <cstdlib>
#include <sstream>
#include <fstream>
#include <iostream>

SystemCommand::SystemCommand() {}
SystemCommand::~SystemCommand() {}

std::string SystemCommand::getDesktopPath()
{
    std::string user = SystemInfo::getUserName();
    return "C:\\Users\\" + user + "\\Desktop\\";
}

std::string SystemCommand::listDesktopFiles()
{
    std::string path = getDesktopPath();
    std::string cmd = "dir \"" + path + "\"";

    FILE *pipe = _popen(cmd.c_str(), "r");
    if (!pipe)
        return "Error: Failed to execute command.";

    std::ostringstream output;
    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr)
    {
        output << buffer;
    }
    _pclose(pipe);
    return output.str();
}

bool SystemCommand::createTextFileOnDesktop(const std::string &filename, const std::string &content)
{
    std::string fullPath = getDesktopPath() + filename;
    std::ofstream file(fullPath);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not create or write to file at " << fullPath << std::endl;
        return false;
    }

    file << content;
    file.close();
    return true;
}

bool SystemCommand::openFileOnDesktop(const std::string &filename)
{
    std::string fullPath = getDesktopPath() + filename;
    std::string cmd = "start \"\" \"" + fullPath + "\"";
    int result = system(cmd.c_str());

    if (result != 0)
    {
        std::cerr << "Error: Failed to open file at " << fullPath << std::endl;
        return false;
    }
    return true;
}

std::string SystemCommand::createAndOpenTextFileOnDesktop(const std::string &filename, const std::string &content)
{
    if (!createTextFileOnDesktop(filename, content))
        return "Error: Failed to create or write to the file.";

    if (!openFileOnDesktop(filename))
        return "File created, but failed to open it with Notepad.";

    return "File successfully created and opened: " + getDesktopPath() + filename;
}

bool SystemCommand::openURLInChrome(const std::string &url)
{
    std::string cmd = "start chrome \"" + url + "\"";
    int result = system(cmd.c_str());
    if (result != 0)
    {
        std::cerr << "Error: Failed to open URL in Chrome: " << url << std::endl;
        return false;
    }
    return true;
}
