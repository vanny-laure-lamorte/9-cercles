#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>

#include "../include/LPTF_Socket.h"
#include "../include/LPTF_Packet.h"

int main() {

    LPTF_Socket socket;
    LPTF_Packet packet;

    if (!socket.create()) {
        std::cerr << "> Error creating server socket\n";
        return 1;
    }

    if (!socket.bind(123)) {
        std::cerr << "> Error binding server socket\n";
        return 1;
    }

    if (!socket.listen()) {
        std::cerr << "> Error setting server to listen\n";
        return 1;
    }

    std::cout << "> Server started on port 123\n";

    std::vector<LPTF_Socket*> clients;
    clients.push_back(&socket);

    std::unordered_map<LPTF_Socket*, std::vector<uint8_t>> recvBuffers;

    while (true) {
        int ready = LPTF_Socket::select_sockets(clients, 1000);

        if (ready > 0) {
            std::vector<LPTF_Socket*> current = clients;

            for (LPTF_Socket* sock : current) {
                if (sock == &socket) {
                    LPTF_Socket* new_client = new LPTF_Socket();
                    if (socket.accept(*new_client)) {
                        std::cout << "> Client connected.\n";
                        clients.push_back(new_client);
                        recvBuffers[new_client] = std::vector<uint8_t>();
                    } else {
                        delete new_client;
                    }
                } else {
                    char buffer[512];
                    int bytes = sock->recvRaw(buffer, sizeof(buffer));
                    if (bytes <= 0) {
                        std::cout << "Client disconnected or recv error\n";
                        sock->close();
                        clients.erase(std::remove(clients.begin(), clients.end(), sock), clients.end());
                        recvBuffers.erase(sock);
                        delete sock;
                    } else {
                        std::string msg(buffer, bytes);
                        std::cout << "> Message from client: " << msg << "\n";

                        std::cout << "> Received raw bytes (" << bytes << "): ";
                        for (int i = 0; i < bytes; i++) {
                            std::cout << std::hex << (unsigned int)(unsigned char)buffer[i] << " ";
                        }
                        std::cout << std::dec << "\n";
                    }
                }
            }
        }
    }

    for (auto* sock : clients) {
        if (sock != &socket) {
            sock->close();
            delete sock;
        }
    }
    return 0;
}
