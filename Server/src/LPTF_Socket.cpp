#include "../include/lptf_socket.h"

LPTF_Socket::LPTF_Socket() : _sockfd(INVALID_SOCKET)
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        cerr << "WSAStartup failed\n";
    }
    else
    {
        cout << "WSAStartup succeeded\n";
    }
}

LPTF_Socket::~LPTF_Socket()
{
    closeSocket();
    WSACleanup();
}

bool LPTF_Socket::createSocket()
{
    _sockfd = socket(AF_INET, SOCK_STREAM, 0);
    return _sockfd != INVALID_SOCKET;
}

void LPTF_Socket::closeSocket()
{
    if (_sockfd != INVALID_SOCKET)
    {
        closesocket(_sockfd);
        _sockfd = INVALID_SOCKET;
    }
}

bool LPTF_Socket::sendMessage(const void* data, int len) {
    return send(_sockfd, static_cast<const char*>(data), len, 0) != SOCKET_ERROR;
}
int LPTF_Socket::receiveMessage(void* buffer, int len) {
    return recv(_sockfd, static_cast<char*>(buffer), len, 0);
}

bool LPTF_Socket::bindSocket(const string &ip, int port)
{
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) != 1)
    {
        cerr << "Invalid IP address\n";
        return false;
    }

    return bind(_sockfd, (SOCKADDR *)&addr, sizeof(addr)) != SOCKET_ERROR;
}

bool LPTF_Socket::listenSocket(int backlog)
{
    return listen(_sockfd, backlog) != SOCKET_ERROR;
}

SOCKET LPTF_Socket::acceptConnection()
{
    return accept(_sockfd, nullptr, nullptr);
}

SOCKET LPTF_Socket::getSocket() const
{
    return _sockfd;
}
