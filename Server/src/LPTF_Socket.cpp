#include "../include/LPTF_Socket.h"
using namespace std;

#include <iostream>

int LPTF_Socket::socket_count = 0;

LPTF_Socket::LPTF_Socket() : sockfd(INVALID_SOCKET) {
    if (socket_count == 0) {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            cerr << "WSAStartup failed.\n";
        }
    }
    ++socket_count;
}

LPTF_Socket::~LPTF_Socket() {
    close();
    --socket_count;
    if (socket_count == 0) {
        WSACleanup();
    }
}

bool LPTF_Socket::create() {
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    return sockfd != INVALID_SOCKET;
}

bool LPTF_Socket::bind(int port) {
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    return ::bind(sockfd, (sockaddr*)&addr, sizeof(addr)) != SOCKET_ERROR;
}

bool LPTF_Socket::listen() {
    return ::listen(sockfd, SOMAXCONN) != SOCKET_ERROR;
}

bool LPTF_Socket::accept(LPTF_Socket& newSock) {
    sockaddr_in clientAddr;
    int addrLen = sizeof(clientAddr);
    SOCKET client_fd = ::accept(sockfd, (sockaddr*)&clientAddr, &addrLen);

    if (client_fd == INVALID_SOCKET)
        return false;

    newSock.sockfd = client_fd;
    return true;
}

bool LPTF_Socket::connect(const string& host, int port) {
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);

    if (inet_pton(AF_INET, host.c_str(), &serverAddr.sin_addr) <= 0) {
        return false;
    }

    return ::connect(sockfd, (sockaddr*)&serverAddr, sizeof(serverAddr)) != SOCKET_ERROR;
}

int LPTF_Socket::send(const string& data) {
    return ::send(sockfd, data.c_str(), static_cast<int>(data.size()), 0);
}

int LPTF_Socket::recv(string& data) {
    char buffer[1024];
    int bytes = ::recv(sockfd, buffer, sizeof(buffer) - 1, 0);

    if (bytes > 0) {
        buffer[bytes] = '\0';
        data = buffer;
    }

    return bytes;
}

void LPTF_Socket::close() {
    if (sockfd != INVALID_SOCKET) {
        closesocket(sockfd);
        sockfd = INVALID_SOCKET;
    }
}

SOCKET LPTF_Socket::get_fd() const {
    return sockfd;
}

int LPTF_Socket::select_sockets(vector<LPTF_Socket*>& sockets, int timeout_ms) {
    fd_set readfds;
    FD_ZERO(&readfds);

    SOCKET max_fd = 0;
    for (auto* sock : sockets) {
        FD_SET(sock->get_fd(), &readfds);
        if (sock->get_fd() > max_fd) max_fd = sock->get_fd();
    }

    timeval timeout;
    timeout.tv_sec = timeout_ms / 1000;
    timeout.tv_usec = (timeout_ms % 1000) * 1000;

    int result = select(0, &readfds, nullptr, nullptr, &timeout);

    if (result > 0) {
        vector<LPTF_Socket*> ready_sockets;

        for (auto* sock : sockets) {
            if (FD_ISSET(sock->get_fd(), &readfds)) {
                ready_sockets.push_back(sock);
            }
        }

        sockets = ready_sockets;
    }

    return result;
}
