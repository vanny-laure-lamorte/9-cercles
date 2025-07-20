#include "../include/LPTF_Socket.h"

int main() {
    std::string ip;
    std::cout << "Enter server IP: ";
    std::cin >> ip;

    LPTF_Socket client;
    if (!client.createSocket()) return 1;
    if (!client.connectToServer(ip, 12345)) {
        std::cerr << "Connection failed\n";
        return 1;
    }

    client.sendMessage("Hello from client!");

    std::string response = client.receiveMessage();
    if (!response.empty()) {
        std::cout << "Server says: " << response << "\n";
    }

    return 0;
}
