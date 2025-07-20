#include "../include/lptf_socket.h"

int main() {
    LPTF_Socket server;
    if (!server.createSocket()) return 1;
    if (!server.bindSocket("127.0.0.1", 12345)) return 1;
    if (!server.listenSocket(1)) return 1;

    std::cout << "Server listening on 127.0.0.1:12345\n";

    SOCKET clientSocket = server.acceptConnection();
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Accept failed.\n";
        return 1;
    }

    char buffer[1024] = {};
    int received = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (received > 0) {
        std::cout << "Client says: " << buffer << "\n";
        std::string response = "Hello client!";
        send(clientSocket, response.c_str(), response.length(), 0);
    }

    closesocket(clientSocket);
    return 0;
}
