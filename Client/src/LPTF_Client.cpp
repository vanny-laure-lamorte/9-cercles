#include "LPTF_Client.h"
#include "SystemInfo.h"

LPTF_Client::LPTF_Client(const string &ip, int port)
{
    SystemInfo systemInfo;

   systemInfo.getUserName();
   systemInfo.getOSInfo();

    if (!socket.create())
    {
        throw runtime_error("Socket creation failed");
    }
    if (!socket.connect(ip.c_str(), port))
    {
        throw runtime_error("Connection failed");
    }
    cout << endl << "Connected to server ||  Ip : " << ip << " || Port : " << to_string(port) << endl;
    cout << "Enter a message (or \"exit\")\n\n";

}

LPTF_Client::~LPTF_Client()
{
    socket.close();
}

void LPTF_Client::sendPacketFromString(const string &message)
{
    vector<uint8_t> payload(message.begin(), message.end());
    LPTF_Packet packet(0x05, payload);
    auto serialized = packet.serialize();
    if (!socket.sendAll(serialized.data(), serialized.size())) {
        cerr << "Error: Failed to send the full packet." << endl;
    }
}

void LPTF_Client::receivePacketAndPrint()
{
    uint8_t header[2];
    if (!socket.receiveAllPackets(header, 2)) {
        cerr << "Error: Failed to receive packet header." << endl;
        return;
    }

    uint16_t packetSize = (header[0] << 8) | header[1];
    if (packetSize < 3) {
        cerr << "Error: Invalid packet size." << endl;
        return;
    }

    vector<uint8_t> data(packetSize);
    data[0] = header[0];
    data[1] = header[1];

    if (!socket.receiveAllPackets(data.data() + 2, packetSize - 2)) {
        cerr << "Error: Failed to receive the full packet." << endl;
        return;
    }

    try {
        LPTF_Packet response = LPTF_Packet::deserialize(data);
        string reply(response.getPayload().begin(), response.getPayload().end());
        cout << "[Server] : " << reply << "\n";
    } catch (const exception& e) {
        cerr << "Error: Deserialization failed - " << e.what() << endl;
    }
}

void LPTF_Client::run()
{
    string input;
    while (true) {
    cout << "[Client] : ";
        getline(cin, input);
        if (input == "exit")
            break;
        sendPacketFromString(input);
        receivePacketAndPrint();
    }
}
