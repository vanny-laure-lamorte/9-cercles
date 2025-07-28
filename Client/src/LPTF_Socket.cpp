#include "LPTF_Socket.h"

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

bool LPTF_Socket::sendAll(const void *data, int len)
{
    const char *ptr = static_cast<const char *>(data);
    int total_sent = 0;

    while (total_sent < len)
    {
        int sent = send(sockfd, ptr + total_sent, len - total_sent, 0);
        if (sent <= 0)
        {
            return false;
        }
        total_sent += sent;
    }
    return true;
}

bool LPTF_Socket::receiveAllPackets(void *buffer, int len)
{
    char *ptr = static_cast<char *>(buffer);
    int total_received = 0;

    while (total_received < len)
    {
        int received = recv(sockfd, ptr + total_received, len - total_received, 0);
        if (received <= 0)
        {
            return false;
        }
        total_received += received;
    }

    return true;
}