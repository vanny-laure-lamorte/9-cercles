#include "SystemCommand.h"
#include "SystemInfo.h"

#include <cstdlib>
#include <sstream>
#include <fstream>
#include <iostream>

SystemCommand::SystemCommand() {}
SystemCommand::~SystemCommand() {}

string SystemCommand::getDesktopPath()
{
    string user = SystemInfo::getUserName();

    return "C:\\Users\\" + user + "\\Desktop\\";
}

string SystemCommand::listDesktopFiles()
{
    string path = getDesktopPath();
    string cmd = "dir \"" + path + "\"";

    FILE *pipe = _popen(cmd.c_str(), "r");
    if (!pipe)
        return "Error: Failed to execute command.";

    ostringstream output;
    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr)
    {
        output << buffer;
    }
    _pclose(pipe);

    return output.str();
}

bool SystemCommand::createTextFileOnDesktop(const string &filename, const string &content)
{
    string fullPath = getDesktopPath() + filename;
    ofstream file(fullPath);
    if (!file.is_open())
    {
        cerr << "Error: Could not create or write to file at " << fullPath << endl;
        return false;
    }

    file << content;
    file.close();

    return true;
}

bool SystemCommand::openFileOnDesktop(const string &filename)
{
    string fullPath = getDesktopPath() + filename;
    string cmd = "start \"\" \"" + fullPath + "\"";
    int result = system(cmd.c_str());

    if (result != 0)
    {
        cerr << "Error: Failed to open file at " << fullPath << endl;
        return false;
    }

    return true;
}

string SystemCommand::createAndOpenTextFileOnDesktop(const string &filename, const string &content)
{
    if (!createTextFileOnDesktop(filename, content))
        return "Error: Failed to create or write to the file.";

    if (!openFileOnDesktop(filename))
        return "File created, but failed to open it with Notepad.";

    return "File successfully created and opened: " + getDesktopPath() + filename;
}

bool SystemCommand::openURLInChrome(const string &url)
{
    string cmd = "start chrome \"" + url + "\"";
    int result = system(cmd.c_str());
    if (result != 0)
    {
        cerr << "Error: Failed to open URL in Chrome: " << url << endl;
        return false;
    }

    return true;
}
