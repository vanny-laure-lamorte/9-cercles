#include "../include/LPTF_Socket.h"

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

    std::cout << "> Connected to server : " << server_ip << endl;
    std::cout << "> Server port :" << server_port << endl;

    if (clientSocket.send("Hello server!") <= 0) {
        std::cerr << "> Send error\n";
        clientSocket.close();
        return 1;
    }

    std::string response;
    int received = clientSocket.recv(response);

    if (received > 0) {
        std::cout << "> Response received: " << response << "\n";
    } else if (received == 0) {
        std::cout << "> Connection closed by server\n";
    } else {
        std::cerr << "> Receive error\n";
    }

    clientSocket.close();

    std::cout << "> Connection closed.\n";
    return 0;
}
