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
        string message(packet.getPayload().begin(), packet.getPayload().end());
        cout << "[CLIENT " << clientSocket.get_fd() << " MSG] : " << message << endl;

        sendPacketToClient(clientSocket, message, CommandType::SEND_MESSAGE);
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
        cout << "\n[SERVER MENU] list/send/quit): \n"
             << "1. list - List connected clients\n"
             << "2. send - Send a message to a client\n"
             << "3. quit - Quit the server\n"
             << endl;
        cin >> input;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "[SERVER MENU] Invalid input. Please enter a number.\n";
            return;
        }

        switch (input)
        {
        case 1:
        {
            cout << "Connected clients:\n";
            for (auto *client : clientSockets)
            {
                cout << "- fd: " << client->get_fd() << "\n";
            }
            break;
        }
        case 2:
        {
            cout << "Enter client fd: ";
            string sfd;
            getline(cin, sfd);
            SOCKET target_fd = static_cast<SOCKET>(stoi(sfd));

            cout << "Enter message to send: ";
            string msg;
            getline(cin, msg);

            for (auto *client : clientSockets)
            {
                if (client->get_fd() == target_fd)
                {
                    LPTF_Packet packet(0x01, vector<uint8_t>(msg.begin(), msg.end()));
                    auto serialized = packet.serialize();
                    client->sendAll(serialized.data(), serialized.size());
                    cout << "[SERVER] Sent to client " << target_fd << "\n";
                    break;
                }
            }
            break;
        }
        case 3:
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
