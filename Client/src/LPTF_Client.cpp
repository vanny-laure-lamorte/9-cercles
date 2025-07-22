#include "LPTF_Client.h"
#include <iostream>

LPTF_Client::LPTF_Client(const string& ip, int port) {
    if (!socket.createSocket()) {
    throw runtime_error("Socket creation failed");
}
    if (!socket.connectToServer(ip.c_str(), port)) {
        throw runtime_error("Connection failed");
    }
}

void LPTF_Client::sendPacketFromString(const string& message) {
    vector<uint8_t> payload(message.begin(), message.end());
    LPTF_Packet packet(0x05, payload);
    auto serialized = packet.serialize();
    socket.sendMessage(serialized.data(), serialized.size());
}

void LPTF_Client::receivePacketAndPrint() {
    uint8_t buffer[1024];
    int received = socket.receiveMessage(buffer, sizeof(buffer));
    if (received > 0) {
        vector<uint8_t> data(buffer, buffer + received);
        LPTF_Packet response = LPTF_Packet::deserialize(data);
        string reply(response.getPayload().begin(), response.getPayload().end());
        cout << "server response : " << reply << "\n";
    }
}

void LPTF_Client::run() {
    string input;
    while (true) {
        cout << "Enter a message (or \"exit\") : ";
        getline(cin, input);
        if (input == "exit") break;

        sendPacketFromString(input);
        receivePacketAndPrint();
    }
}
