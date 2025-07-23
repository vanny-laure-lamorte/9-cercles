#include "LPTF_Server.h"
#include <iostream>

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

    cout << "Server running and waiting" <<  endl;
    cout << "Ip : "<< ip << " | Port : " << port << endl;
    handleConnection();
}

LPTF_Server::~LPTF_Server()
{
}

void LPTF_Server::run()
{
    while (true)
    {
        for (auto it = clientSockets.begin(); it != clientSockets.end();)
        {
            LPTF_Socket *client = *it;
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
                cout << "Client disconnected! (" << clientSockets.size() << " total)\n";
                handleConnection();
            }
            else
            {
                ++it;
            }
        }
    }
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
        cout << "[CLIENT MSG] : " << message << endl;

        sendResponse(clientSocket, message);
    }
    catch (const std::exception &e)
    {
        cerr << "Packet error: " << e.what() << endl;
    }
}

void LPTF_Server::sendResponse(LPTF_Socket &clientSocket, const std::string &message)
{
    vector<uint8_t> payload(message.begin(), message.end());
    LPTF_Packet response(0x05, payload);
    auto serialized = response.serialize();

    if (!clientSocket.sendAll(serialized.data(), serialized.size()))
    {
        cerr << "Error sending response to client." << endl;
    }
}

void LPTF_Server::handleConnection()
{
    SOCKET newSock = serverSocket.accept();
    cout << "New client connection attempt..." << endl;
    if (newSock != INVALID_SOCKET)
    {
        auto *newClient = new LPTF_Socket();
        newClient->setSocket(newSock);
        clientSockets.push_back(newClient);
        cout << "New client connected! (" << clientSockets.size() << " total)\n";
    }
}