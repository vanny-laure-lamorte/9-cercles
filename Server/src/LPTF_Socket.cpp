#include "../include/LPTF_Socket.h"
using namespace std;

#include <iostream>

int LPTF_Socket::socket_count = 0;

LPTF_Socket::LPTF_Socket() : sockfd(INVALID_SOCKET)
{
    if (socket_count == 0)
    {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        {
            cerr << "WSAStartup failed.\n";
        }
    }
    ++socket_count;
}

LPTF_Socket::~LPTF_Socket()
{
    close();
    --socket_count;
    if (socket_count == 0)
    {
        WSACleanup();
    }
}

bool LPTF_Socket::create()
{
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    return sockfd != INVALID_SOCKET;
}

bool LPTF_Socket::bind(int port)
{
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    return ::bind(sockfd, (sockaddr *)&addr, sizeof(addr)) != SOCKET_ERROR;
}

bool LPTF_Socket::listen()
{
    return ::listen(sockfd, SOMAXCONN) != SOCKET_ERROR;
}

SOCKET LPTF_Socket::accept()
{
    cout << "Accepting new connection..." << endl;
    sockaddr_in clientAddr;
    int addrLen = sizeof(clientAddr);
    return ::accept(sockfd, (sockaddr *)&clientAddr, &addrLen);
}

bool LPTF_Socket::connect(const string &host, int port)
{
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);

    if (inet_pton(AF_INET, host.c_str(), &serverAddr.sin_addr) <= 0)
    {
        return false;
    }

    return ::connect(sockfd, (sockaddr *)&serverAddr, sizeof(serverAddr)) != SOCKET_ERROR;
}

bool LPTF_Socket::sendMsg(const void *data, int len)
{
    return send(sockfd, static_cast<const char *>(data), len, 0) != SOCKET_ERROR;
}
int LPTF_Socket::receivePacket(void *buffer, int len)
{
    return recv(sockfd, static_cast<char *>(buffer), len, 0);
}

bool LPTF_Socket::receiveAllPackets(void *buffer, int len)
{
    char *ptr = static_cast<char *>(buffer);
    int totalReceived = 0;
    while (totalReceived < len)
    {
        int received = recv(sockfd, ptr + totalReceived, len - totalReceived, 0);
        if (received <= 0)
            return false;
        totalReceived += received;
    }
    return true;
}

bool LPTF_Socket::sendAll(const void* data, int len) {
    const char* ptr = static_cast<const char*>(data);
    int totalSent = 0;
    while (totalSent < len) {
        int sent = send(sockfd, ptr + totalSent, len - totalSent, 0);
        if (sent <= 0) return false;
        totalSent += sent;
    }

    return true;
}

void LPTF_Socket::close()
{
    if (sockfd != INVALID_SOCKET)
    {
        closesocket(sockfd);
        sockfd = INVALID_SOCKET;
    }
}

SOCKET LPTF_Socket::get_fd() const
{
    return sockfd;
}

int LPTF_Socket::select_sockets(vector<LPTF_Socket *> &sockets, int timeout_ms)
{
    fd_set readfds;
    FD_ZERO(&readfds);

    SOCKET max_fd = 0;
    for (auto *sock : sockets)
    {
        FD_SET(sock->get_fd(), &readfds);
        if (sock->get_fd() > max_fd)
            max_fd = sock->get_fd();
    }

    timeval timeout;
    timeout.tv_sec = timeout_ms / 1000;
    timeout.tv_usec = (timeout_ms % 1000) * 1000;
    int result = select(0, &readfds, nullptr, nullptr, &timeout);

    if (result > 0)
    {
        vector<LPTF_Socket *> ready_sockets;
        for (auto *sock : sockets)
        {
            if (FD_ISSET(sock->get_fd(), &readfds))
            {
                ready_sockets.push_back(sock);
            }
        }
        sockets = ready_sockets;
    }
    return result;
}

void LPTF_Socket::setSocket(SOCKET newSocket) {
    if (sockfd != INVALID_SOCKET) {
        closesocket(sockfd);
    }
    sockfd = newSocket;
}