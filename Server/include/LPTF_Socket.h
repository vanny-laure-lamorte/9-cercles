#ifndef LPTF_SOCKET_H
#define LPTF_SOCKET_H
using namespace std;

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <winsock2.h>
    #include <windows.h>
    #include <ws2tcpip.h>
#endif

#include <string>
#include <vector>

#include "../include/LPTF_Packet.h"

class LPTF_Socket {
public:

    //  Constructor: Initializes the socket object
    LPTF_Socket();
    //  Destructor: Cleans up the socket object
    ~LPTF_Socket();

    //  Creates a TCP socket
    bool create();

    //  Binds the socket to a port
    bool bind(int port);

    //  Sets the socket to listen for incoming connections
    bool listen();

    //  Accepts a new incoming connection and assigns it to newSock
    bool accept(LPTF_Socket& newSock);

    //  Connects to a server at the specified host and port
    bool connect(const string& host, int port);

    //  Sends data through the socket
    bool sendMsg(const void* data, int len);

    //  Receives data from the socket
    int receive(void* buffer, int len);

    //  Closes the socket
    void close();
    SOCKET get_fd() const;

    // Monitors multiple sockets for readiness (read) with a timeout
    static int select_sockets(vector<LPTF_Socket*>& sockets, int timeout_ms);

    // Sends a packet over the socket
    bool sendPacket(const LPTF_Packet& packet);

    // Receive packet
    bool recvPacket(LPTF_Packet& packet, std::vector<uint8_t>& buffer);

    int recvRaw(void* buffer, int len);

private:

    // The socket handle
    SOCKET sockfd;

    // Count of active sockets for managing Winsock startup/cleanup
    static int socket_count;
};

#endif
