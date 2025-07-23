#ifndef LPTF_SOCKET_H
#define LPTF_SOCKET_H
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <string>
#include <vector>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
using namespace std;

#pragma comment(lib, "ws2_32.lib")

class LPTF_Socket
{
public:
    //  Constructor: Initializes the socket object
    LPTF_Socket();
    //  Destructor: Cleans up the socket object
    ~LPTF_Socket();

    //  Creates a TCP socket
    bool create();

    //  Connects to a server at the specified host and port
    bool connect(const string &host, int port);
    //  Closes the socket
    void close();

    //  Sends data through the socket
    bool sendMsg(const void *data, int len);
    //  Send all data until the specified length is met
    bool sendAll(const void *data, int len);

    //  Receives data from the socket
    int receivePacket(void *buffer, int len);
    //  Receives all data until the specified length is met
    bool receiveAllPackets(void *buffer, int len);

    // Gets the socket file descriptor
    SOCKET get_fd() const;

    // Monitors multiple sockets for readiness (read) with a timeout
    static int select_sockets(vector<LPTF_Socket *> &sockets, int timeout_ms);

private:
    // The socket handl
    SOCKET sockfd;

    // Count of active sockets for managing Winsock startup/cleanup
    static int socket_count;
};

#endif