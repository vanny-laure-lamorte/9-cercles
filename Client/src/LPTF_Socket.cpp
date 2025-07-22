#include "../include/LPTF_Socket.h"

LPTF_Socket::LPTF_Socket() : _sockfd(INVALID_SOCKET) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup failed\n";
    } else {
        cout << "WSAStartup succeeded\n";
    }
}

LPTF_Socket::~LPTF_Socket() {
    closeSocket();
    WSACleanup();
}

bool LPTF_Socket::createSocket() {
    _sockfd = socket(AF_INET, SOCK_STREAM, 0);
    return _sockfd != INVALID_SOCKET;
}

void LPTF_Socket::closeSocket() {
    if (_sockfd != INVALID_SOCKET) {
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

bool LPTF_Socket::connectToServer(const string& ip, int port) {
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr);
    return connect(_sockfd, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) != SOCKET_ERROR;
}

SOCKET LPTF_Socket::getSocket() const {
    return _sockfd;
}
