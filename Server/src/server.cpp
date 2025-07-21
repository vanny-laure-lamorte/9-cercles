#include <iostream>
#include <vector>
#include <algorithm>
#include "../include/LPTF_Socket.h"

int main() {

    LPTF_Socket server;

    if (!server.create()) {
        std::cerr << "> Error creating server socket\n";
        return 1;
    }

    if (!server.bind(123)) {
        std::cerr << "> Error binding server socket\n";
        return 1;
    }

    if (!server.listen()) {
        std::cerr << "> Error setting server to listen\n";
        return 1;
    }

    std::cout << "> Server started on port 123\n";

    std::vector<LPTF_Socket*> clients;
    clients.push_back(&server);

    while (true) {
        int ready = LPTF_Socket::select_sockets(clients, 1000);

        if (ready > 0) {
            std::vector<LPTF_Socket*> current = clients;

            for (LPTF_Socket* sock : current) {
                if (sock == &server) {
                    LPTF_Socket* new_client = new LPTF_Socket();
                    if (server.accept(*new_client)) {
                        std::cout << "> Client connected.\n";
                        clients.push_back(new_client);
                    } else {
                        delete new_client;
                    }
                } else {
                    std::string msg;
                    int received = sock->recv(msg);

                    if (received <= 0) {
                        std::cout << "> Client disconnected.\n";
                        sock->close();
                        clients.erase(std::remove(clients.begin(), clients.end(), sock), clients.end());
                        delete sock;
                    } else {
                        std::cout << "> [Client] " << msg << "\n";
                        sock->send("> Received: " + msg);
                    }
                }
            }
        }
    }

    for (auto* sock : clients) {
        if (sock != &server) {
            sock->close();
            delete sock;
        }
    }

    return 0;
}
