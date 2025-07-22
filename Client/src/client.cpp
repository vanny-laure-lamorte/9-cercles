#include <iostream>
#include <string>
#include <vector>
#include "../include/LPTF_Socket.h"
#include "../include/LPTF_Packet.h"

int main() {
    LPTF_Socket clientSocket;
    if (!clientSocket.create()) {
        std::cerr << "Socket creation error\n";
        return 1;
    }

    const std::string server_ip = "127.0.0.1";
    const int server_port = 123;

    if (!clientSocket.connect(server_ip, server_port)) {
        std::cerr << "Unable to connect to server\n";
        return 1;
    }

    std::cout << "> Connected to server: " << server_ip << ":" << server_port << std::endl;

    std::vector<uint8_t> recvBuffer;

    while (true) {
        std::cout << "Enter message (or 'quit' to exit): ";
        std::string userInput;
        std::getline(std::cin, userInput);

        if (userInput == "quit") {
            break;
        }

        LPTF_Packet packet(1, userInput);
        std::cout << "> Sending message: " << userInput << std::endl;

        if (!clientSocket.sendPacket(packet)) {
            std::cerr << "> Send error\n";
            break;
        }

        LPTF_Packet responsePacket;
        if (!clientSocket.recvPacket(responsePacket, recvBuffer)) {
            std::cerr << "> Receive error or connection closed\n";
            break;
        }

        std::cout << "> Server response (type "
                  << static_cast<int>(responsePacket.getType())
                  << "): " << responsePacket.getPayloadAsString() << std::endl;
    }

    clientSocket.close();
    std::cout << "> Connection closed.\n";
    return 0;
}
