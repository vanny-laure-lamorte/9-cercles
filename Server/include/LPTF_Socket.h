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

class LPTF_Socket
{
public:
    //  Constructor: Initializes the socket object
    LPTF_Socket();
    //  Destructor: Cleans up the socket object
    ~LPTF_Socket();

    // Copy constructor and assignment operator are deleted to prevent copying
    LPTF_Socket(const LPTF_Socket &) = delete;
    // Assignment Operator
    LPTF_Socket &operator=(const LPTF_Socket &) = delete;

    //  Creates a TCP socket
    bool create();
    //  Closes the socket
    void close();

    //  Binds the socket to a port
    bool bind(int port);
    //  Sets the socket to listen for incoming connections
    bool listen();
    // Accepts a new incoming connection and returns the new socket
    SOCKET accept();

    //  Connects to a server at the specified host and port
    bool connect(const string &host, int port);

    //  Sends data through the socket
    bool sendMsg(const void *data, int len);
    // Sends all data through the socket
    bool sendAll(const void* data, int len);

    //  Receives data from the socket
    int receivePacket(void *buffer, int len);
    // Receives all packets from the socket
    bool receiveAllPackets(void *buffer, int len);

    // Getter for the socket file descriptor
    SOCKET get_fd() const;

    // Monitors multiple sockets for readiness (read) with a timeout
    static int select_sockets(vector<LPTF_Socket *> &sockets, int timeout_ms);

    // Set a new socket for a new client
    void setSocket(SOCKET newSocket);

private:
    // The socket file descriptor
    SOCKET sockfd;

    // Count of active sockets for managing Winsock startup/cleanup
    static int socket_count;
};
#endif