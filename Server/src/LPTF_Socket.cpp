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


bool LPTF_Socket::sendMsg(const void* data, int len) {
    return send(sockfd, static_cast<const char*>(data), len, 0) != SOCKET_ERROR;
}

int LPTF_Socket::receive(void* buffer, int len) {
    return recv(sockfd, static_cast<char*>(buffer), len, 0);
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

bool LPTF_Socket::sendPacket(const LPTF_Packet& packet) {
    std::vector<uint8_t> data = packet.serialize();
    size_t totalSent = 0;
    while (totalSent < data.size()) {
        int sent = ::send(sockfd, reinterpret_cast<const char*>(data.data() + totalSent), static_cast<int>(data.size() - totalSent), 0);
        if (sent == SOCKET_ERROR || sent == 0) {
            return false;
        }
        totalSent += sent;
    }
    return true;
}

bool LPTF_Socket::recvPacket(LPTF_Packet& packet, std::vector<uint8_t>& buffer) {

    std::cout << "rcv Packet";
    while (buffer.size() < 3) {
        char temp[512];
        int bytes = ::recv(sockfd, temp, sizeof(temp), 0);
        std::cout << "[recvPacket] recv bytes: " << bytes << "\n";
        if (bytes <= 0) {
            std::cout << "[recvPacket] recv failed or connection closed\n";
            return false;
        }
        buffer.insert(buffer.end(), temp, temp + bytes);
    }

    uint16_t payloadLen = ntohs(*reinterpret_cast<const uint16_t*>(&buffer[1]));
    size_t packetSize = 3 + payloadLen;
    while (buffer.size() < packetSize) {
        char temp[512];
        int bytes = ::recv(sockfd, temp, sizeof(temp), 0);
        std::cout << "[recvPacket] recv bytes: " << bytes << "\n";
        if (bytes <= 0) {
            std::cout << "[recvPacket] recv failed or connection closed\n";
            return false;
        }
        buffer.insert(buffer.end(), temp, temp + bytes);
    }

    size_t consumedBytes = 0;
    try {
        packet = LPTF_Packet::deserialize(buffer.data(), buffer.size(), consumedBytes);
    } catch (const std::exception& e) {
        std::cout << "[recvPacket] deserialize exception: " << e.what() << "\n";
        return false;
    } catch (...) {
        std::cout << "[recvPacket] deserialize unknown exception\n";
        return false;
    }
    buffer.erase(buffer.begin(), buffer.begin() + consumedBytes);
    return true;
}

int LPTF_Socket::recvRaw(void* buffer, int len) {
    #ifdef _WIN32
        return ::recv(sockfd, static_cast<char*>(buffer), len, 0);
    #else
        return ::recv(sockfd, buffer, len, 0);
    #endif
}

