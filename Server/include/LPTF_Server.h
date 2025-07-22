#ifndef LPTF_SERVER_HPP
#define LPTF_SERVER_HPP

#include "LPTF_Socket.h"
#include "LPTF_Packet.h"
#include <string>
#include <vector>
using namespace std;

class LPTF_Server {
public:
    LPTF_Server(const string& ip, int port);
    void run();

private:
    LPTF_Socket serverSocket;
    SOCKET clientSocket;

    void handleClient();
    void sendResponse(const string& message);
};

#endif
