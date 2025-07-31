#include "LPTF_Server.h"

LPTF_Server::LPTF_Server(const string &ip, int port, QObject *parent)
    : QObject(parent), clientSocket(INVALID_SOCKET)
{
    if (!db.connect()) {
        throw runtime_error("Failed to connect to DB");
    }

    cout << "[SERVER] Initializing server with IP:" << ip << "Port:" << port;
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

    qDebug() << "[SERVER] Running and waiting";
    qDebug() << "[SERVER] Ip:" << QString::fromStdString(ip) << "| Port:" << port;

    handleConnection();
    cout << "[handleConnection - SERVER] Server initialized and listening on port:" << port;
}

LPTF_Server::~LPTF_Server()
{
    qDebug() << "[SERVER] Shutdown.";
}

void LPTF_Server::handleClient(LPTF_Socket &clientSocket)
{
    uint8_t header[2];
    if (!clientSocket.receivePacket(header, 2))
    {
        qWarning() << "[SERVER] Error receiving packet header.";
        return;
    }

    uint16_t packetSize = (header[0] << 8) | header[1];
    if (packetSize < 3)
    {
        qWarning() << "[SERVER] Invalid packet size.";
        return;
    }

    vector<uint8_t> data(packetSize);
    data[0] = header[0];
    data[1] = header[1];

    if (!clientSocket.receivePacket(data.data() + 2, packetSize - 2))
    {
        qWarning() << "[SERVER] Error receiving full packet.";
        return;
    }

    try
    {
        LPTF_Packet packet = LPTF_Packet::deserialize(data);
        handleCommand(packet, clientSocket);
        handleCommand(packet, clientSocket);
    }
    catch (const exception &e)
    {
        qWarning() << "[SERVER] Packet error:" << e.what();
    }
}

void LPTF_Server::sendPacketToClient(LPTF_Socket &clientSocket,
                                     const string &data,
                                     CommandType type)
{
    if (data.empty())
    {
        qWarning() << "[SERVER] Empty data, not sending response.";
        return;
    }

    qDebug() << "[SERVER] Sending data to client" << clientSocket.get_fd()
             << ":" << QString::fromStdString(data);

    vector<uint8_t> payload(data.begin(), data.end());
    LPTF_Packet response(static_cast<uint8_t>(type), payload);
    auto serialized = response.serialize();

    if (!clientSocket.sendAll(serialized.data(), serialized.size()))
    {
        qWarning() << "[SERVER] Error sending data to client.";
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
        qDebug() << "[SERVER] Accepting new connection...";
        SOCKET newSock = serverSocket.accept();
        if (newSock != INVALID_SOCKET)
        {
            auto *newClient = new LPTF_Socket();
            newClient->setSocket(newSock);
            clientSockets.push_back(newClient);
            QString socketStr = QString::number(newClient->get_fd());
            emit clientCountChanged(clientSockets.size());
            emit clientConnected(socketStr);
            qDebug() << "[SERVER] Client connected! Total clients:"
                     << clientSockets.size();
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
            qWarning() << "[SERVER ERROR] select() failed.";
            continue;
        }

        for (auto it = clientSockets.begin(); it != clientSockets.end();)
        {
            LPTF_Socket *client = *it;

            if (FD_ISSET(client->get_fd(), &readfds))
            {
                uint8_t header[2];
                int result = recv(client->get_fd(),
                                  reinterpret_cast<char *>(header), 2, MSG_PEEK);

                if (result > 0)
                {
                    handleClient(*client);
                    ++it;
                }
                else if (result == 0 ||
                         (result == -1 && WSAGetLastError() == WSAECONNRESET))
                {
                    QString socketStr = QString::number(client->get_fd());
                    client->close();
                    qDebug() << "Deleting client object...";
                    it = clientSockets.erase(it);
                    delete client;
                    qDebug() << "Client deleted.";
                    client = nullptr;

                    emit clientCountChanged(clientSockets.size());
                    emit clientDisconnected(socketStr);
                    qDebug() << "[SERVER] Client disconnected. Total:"
                             << clientSockets.size();
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
                  << "3. Return system information\n"
                  << "4. List running processes\n"
                  << "5. Execute a system command\n"
                  << "8. quit - Quit the server\n\n"
                  << ">> Choose an option: ";
        cin >> input;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "[SERVER MENU] Invalid input. Please enter a number.\n";
            return;
        }

        for (auto *client : clientSockets)
        {
            switch (input)
            {
            case 1:
                cout << "Connected clients:\n";
                cout << "- fd: " << client->get_fd() << "\n";
                break;
            case 3:
                sendPacketToClient(*client, "> Requesting host info",
                                   CommandType::HOST_INFO_REQUEST);
                break;
            case 4:
                sendPacketToClient(*client, "> Requesting process list",
                                   CommandType::LIST_PROCESSES_REQUEST);
                break;
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
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
                qDebug() << "[SERVER] Shutting down...";
                exit(0);
            default:
                cout << "[SERVER MENU] Unknown command.\n";
                break;
            }
        }
    }
}

void LPTF_Server::handleCommand(const LPTF_Packet &packet, LPTF_Socket &clientSocket)
{
    string data(packet.getPayload().begin(), packet.getPayload().end());
    CommandType type = static_cast<CommandType>(packet.getType());
    QString socketStr = QString::number(clientSocket.get_fd());

    switch (type)
    {
    case CommandType::HOST_INFO_RESPONSE:
    {
        cout << data << endl;
        emit systemInfoReceived(QString::fromStdString(data), socketStr);
        qDebug() << "[SERVER] Host info received, emitting signal.";
        break;
    }
    case CommandType::LIST_PROCESSES_RESPONSE:
    {
        auto processes = deserializeStringTable(packet.getPayload());
        qDebug() << "[SERVER] Process list received.";
        for (const auto &process : processes)
        {
            cout << "Process: " << left << setw(20) << process[0].substr(0, 18)
                 << " PID: " << setw(6) << process[1]
                 << " | PPID: " << setw(6) << process[2]
                 << " | Threads: " << setw(4) << process[3]
                 << " | Priority: " << setw(3) << process[4]
                 << " | Runtime: " << process[5] << endl;
        }
        // QString socketStr = QString::number(clientSocket->get_fd());
        emit processListReceived(processes, QString(socketStr));
        break;
    }

    case CommandType::COMMAND_RESULT_RESPONSE:
    {
        cout << data << endl;

        break;
    }
    case CommandType::SEND_MESSAGE:
    {
        cout << "[SERVER] Message from client " << socketStr.toStdString() << ": " << data << endl;
        break;
    }
    default:
        qWarning() << "[SERVER] Unknown command type received:"
                   << static_cast<int>(packet.getType());
        break;
    }
}

vector<vector<string>>
LPTF_Server::deserializeStringTable(const vector<uint8_t> &payload)
{
    vector<vector<string>> result;
    size_t offset = 0;

    auto read_uint32 = [&](uint32_t &value)
    {
        if (offset + sizeof(uint32_t) > payload.size())
            throw runtime_error("Invalid payload");
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
                throw runtime_error("Invalid payload");

            string cell(payload.begin() + offset,
                             payload.begin() + offset + strLen);
            offset += strLen;

            row.push_back(cell);
        }
        result.push_back(row);
    }

    return result;
}

void LPTF_Server::sendSystemInfoRequest()
{
    qDebug() << "[SERVER] Sending system info request to clients...";
    if (clientSockets.empty())
    {
        qDebug() << "[SERVER] No client connected.";
        return;
    }

    for (auto *client : clientSockets)
    {
        sendPacketToClient(*client, "> Requesting host info",
                           CommandType::HOST_INFO_REQUEST);
    }
}

void LPTF_Server::sendProcessListRequestFor(const QString &socketId)
{
    for (auto *client : clientSockets)
    {
        if (QString::number(client->get_fd()) == socketId)
        {
            sendPacketToClient(*client, "> Requesting process list",
                               CommandType::LIST_PROCESSES_REQUEST);
            qDebug() << "[SERVER] Sent processes list request to client:" << socketId;
            return;
        }
    }
    qWarning() << "[SERVER] No client found for socketId:" << socketId;
    if (clientSockets.empty())
    {
        qDebug() << "[SERVER] No client connected.";
        return;
    }
}

void LPTF_Server::sendSystemInfoRequestFor(const QString &socketId)
{
    for (auto *client : clientSockets)
    {
        if (QString::number(client->get_fd()) == socketId)
        {
            sendPacketToClient(*client, "> Requesting host info",
                               CommandType::HOST_INFO_REQUEST);
            qDebug() << "[SERVER] Sent system info request to client:" << socketId;
            return;
        }
    }
    qWarning() << "[SERVER] No client found for socketId:" << socketId;
}
