#ifndef LPTF_CLIENT_HPP
#define LPTF_CLIENT_HPP

#include "LPTF_Socket.h"
#include "LPTF_Packet.h"
#include <string>
#include <iostream>
using namespace std;

class LPTF_Client {
public:
    explicit LPTF_Client(const std::string& ip, int port);
    void run();
private:
    LPTF_Socket socket;

    void sendPacketFromString(const std::string& message);
    void receivePacketAndPrint();
};

#endif
