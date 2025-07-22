#ifndef LPTF_SOCKET_HPP
#define LPTF_SOCKET_HPP

#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <iostream>
using namespace std;

#pragma comment(lib, "ws2_32.lib")

class LPTF_Socket {
public:
    LPTF_Socket();
    ~LPTF_Socket();
    LPTF_Socket(const LPTF_Socket&) = delete;
    LPTF_Socket& operator=(const LPTF_Socket&) = delete;

    bool createSocket();
    void closeSocket();
    bool sendMessage(const void* data, int len);
    int receiveMessage(void* buffer, int len);
    bool connectToServer(const std::string& ip, int port);

    SOCKET getSocket() const;

private:
    SOCKET _sockfd;
};

#endif
