#ifndef LPTF_SERVER_HPP
#define LPTF_SERVER_HPP

#include "LPTF_Socket.h"
#include "LPTF_Packet.h"
#include <thread>
#include <string>
#include <vector>
#include <conio.h>

using namespace std;

class LPTF_Server
{
public:
    // Constructor: Initializes the server with the specified IP and port
    LPTF_Server(const string &ip, int port);
    // Destructor : Cleans up the server resources
    ~LPTF_Server();

    // Copy constructor and assignment operator are deleted to prevent copying
    LPTF_Server(const LPTF_Server &) = delete;
    // Assignment Operator
    LPTF_Server &operator=(const LPTF_Server &) = delete;

    // Starts the server and listens for incoming connections
    void run();

private:
    // The IP address and port for the server
    LPTF_Socket serverSocket;

    // The IP address and port for the server for a client socket
    SOCKET clientSocket;

    // Vector of client sockets
    vector<LPTF_Socket *> clientSockets;

    // Send packet(s) and wait for the server to respond
    void handleClient(LPTF_Socket &clientSocket);

    // Handles a new client connection
    void handleConnection();

    // Receives a packet from the client
    void sendResponse(LPTF_Socket &clientSocket, const std::string &message);

    void handleAdminInput();

};
#endif