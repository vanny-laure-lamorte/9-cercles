#ifndef LPTF_CLIENT_HPP
#define LPTF_CLIENT_HPP

#include "LPTF_Socket.h"
#include "LPTF_Packet.h"
#include <string>
#include <iostream>
using namespace std;

class LPTF_Client {
public:
    // Constructor that initializes the socket and connects to the server
    explicit LPTF_Client(const std::string& ip, int port);
    // Destructor
    ~LPTF_Client();

    // Disable copy constructor and assignment operator
    LPTF_Client(const LPTF_Client&) = delete;
    // Disable assignment operator
    LPTF_Client& operator=(const LPTF_Client&) = delete;

    // Main loop for the client
    // It reads user input, sends packets, and receives responses
    void run();
private:
    // Socket object for communication
    LPTF_Socket socket;

    // Sends a packet constructed from a string message
    void sendPacketFromString(const std::string& message);
    // Receives a packet from the server and prints the response
    void receivePacketAndPrint();
};

#endif
