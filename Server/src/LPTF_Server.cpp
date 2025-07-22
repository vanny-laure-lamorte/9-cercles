#include "LPTF_Server.h"
#include <iostream>

LPTF_Server::LPTF_Server(const string& ip, int port) : clientSocket(INVALID_SOCKET) {
    if (!serverSocket.createSocket()) {
        throw runtime_error("Failed to create server socket");
    }
    if (!serverSocket.bindSocket(ip, port)) {
        throw runtime_error("Bind failed");
    }
    if (!serverSocket.listenSocket(1)) {
        throw runtime_error("Listen failed");
    }

    cout << "Server waiting on " << ip << ":" << port << endl;
    clientSocket = serverSocket.acceptConnection();
    if (clientSocket == INVALID_SOCKET) {
        throw runtime_error("Failed to accept connection");
    }
    cout << "Client connected !" << endl;
}

void LPTF_Server::run() {
    while(true) {
        handleClient();
    }
}

void LPTF_Server::handleClient() {
    uint8_t buffer[1024];
    int received = recv(clientSocket, reinterpret_cast<char*>(buffer), sizeof(buffer), 0);

    if (received > 0) {
        vector<uint8_t> data(buffer, buffer + received);
        LPTF_Packet packet = LPTF_Packet::deserialize(data);

        string message(packet.getPayload().begin(), packet.getPayload().end());
        cout << "Received Message : " << message << endl;

        sendResponse(message);
    }
}

void LPTF_Server::sendResponse(const string& message) {
    vector<uint8_t> payload(message.begin(), message.end());
    LPTF_Packet response(0x05, payload);
    auto serialized = response.serialize();
    send(clientSocket, reinterpret_cast<const char*>(serialized.data()), serialized.size(), 0);
}
