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

    cout << "Server waiting on " << ip << ":" << port << endl;
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

LPTF_Server::~LPTF_Server()
{
}

void LPTF_Server::run()
{
    while (true)
    {
        // SOCKET newSock = serverSocket.accept();
        // cout << "New client connection attempt..." << endl;
        // if (newSock != INVALID_SOCKET)
        // {
        //     auto *newClient = new LPTF_Socket();
        //     newClient->setSocket(newSock);
        //     clientSockets.push_back(newClient);
        //     cout << "New client connected! (" << clientSockets.size() << " total)\n";
        //
        for (auto *client : clientSockets)
        {
            cout << ">>>>>> 1- Checking client readiness..." << endl;
            uint8_t header[2];
            cout << ">>>>> 2- Peeking header..." << endl;
            int result = recv(client->get_fd(), reinterpret_cast<char *>(header), 2, MSG_PEEK);
            cout << ">>>>>> 3- Peek result: " << result << endl;
            if (result > 0)
            {
                cout << ">>>>> 4- Client ready to read data." << endl;
                handleClient(*client);
                cout << ">>>>> 5- Client handled." << endl;
            }
        }
    }
}

void LPTF_Server::handleClient(LPTF_Socket &clientSocket)
{
    uint8_t header[2];
    cout << ">>>>> handleClient 1- Receiving packet header..." << endl;
    if (!clientSocket.receivePacket(header, 2))
    {
        cerr << "Error receiving packet header." << endl;
        return;
    }
    cout << ">>>>> handleClient 2- Header received: " << static_cast<int>(header[0]) << ", " << static_cast<int>(header[1]) << endl;
    uint16_t packetSize = (header[0] << 8) | header[1];
    if (packetSize < 3)
    {
        cerr << "Invalid packet size." << endl;
        return;
    }
    cout << ">>>>> handleClient 3- Packet size: " << packetSize << endl;
    vector<uint8_t> data(packetSize);
    data[0] = header[0];
    data[1] = header[1];
    cout << ">>>>> handleClient 4- Receiving full packet..." << endl;
    if (!clientSocket.receivePacket(data.data() + 2, packetSize - 2))
    {
        cerr << "Error receiving full packet." << endl;
        return;
    }
    cout << ">>>>> handleClient 5- Full packet received, size: " << data.size() << endl;
    try
    {
        LPTF_Packet packet = LPTF_Packet::deserialize(data);
        string message(packet.getPayload().begin(), packet.getPayload().end());
        cout << "Received from client: " << message << endl;

        vector<uint8_t> payload(message.begin(), message.end());
        LPTF_Packet response(0x05, payload);
        auto serialized = response.serialize();
        clientSocket.sendAll(serialized.data(), serialized.size());
    }
    catch (const std::exception &e)
    {
        cerr << "Packet error: " << e.what() << endl;
    }
}

void LPTF_Server::sendResponse(const string &message)
{
    vector<uint8_t> payload(message.begin(), message.end());
    LPTF_Packet response(0x05, payload);

    auto serialized = response.serialize();

    if (!serverSocket.sendAll(serialized.data(), serialized.size()))
    {
        cerr << "Erreur lors de l'envoi de la réponse." << endl;
    }
}
