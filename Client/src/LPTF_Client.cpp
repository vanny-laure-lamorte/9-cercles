#include "LPTF_Client.h"

LPTF_Client::LPTF_Client(const string &ip, int port)
{
    SystemInfo systemInfo;
    cout << systemInfo.getAllInfo() << endl;

    ProcessManager pm;
    cout << pm.getRunningProcesses() << endl;

    if (!socket.create())
    {
        throw runtime_error("Socket creation failed");
    }
    if (!socket.connect(ip.c_str(), port))
    {
        throw runtime_error("Connection failed");
    }
    cout << endl
         << "Connected to server ||  Ip : " << ip << " || Port : " << to_string(port) << endl;
    cout << "Enter a message (or \"exit\")\n\n";
}

LPTF_Client::~LPTF_Client()
{
    socket.close();
}

void LPTF_Client::sendPacketFromString(const string &message, CommandType type)
{
    vector<uint8_t> payload(message.begin(), message.end());
    LPTF_Packet packet(static_cast<uint8_t>(type), payload);
    auto serialized = packet.serialize();
    if (!socket.sendAll(serialized.data(), serialized.size()))
    {
        cerr << "Error: Failed to send the full packet." << endl;
    }
}

void LPTF_Client::receivePacketAndPrint()
{
    uint8_t header[2];
    if (!socket.receiveAllPackets(header, 2))
    {
        cerr << "Error: Failed to receive packet header." << endl;
        return;
    }

    uint16_t packetSize = (header[0] << 8) | header[1];
    if (packetSize < 3)
    {
        cerr << "Error: Invalid packet size." << endl;
        return;
    }

    vector<uint8_t> data(packetSize);
    data[0] = header[0];
    data[1] = header[1];

    if (!socket.receiveAllPackets(data.data() + 2, packetSize - 2))
    {
        cerr << "Error: Failed to receive the full packet." << endl;
        return;
    }
    try
    {
        LPTF_Packet response = LPTF_Packet::deserialize(data);
        string reply(response.getPayload().begin(), response.getPayload().end());
        cout << "[Server] : " << reply << "\n";
    }
    catch (const exception &e)
    {
        cerr << "Error: Deserialization failed - " << e.what() << endl;
    }
}

void LPTF_Client::sendProcessList()
{
    ProcessManager pm;
    string processes = pm.getRunningProcesses();
    sendPacketFromString(processes, CommandType::PROCESS_LIST_RESPONSE);
}

void LPTF_Client::run()
{
    while (true)
    {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(socket.get_fd(), &readfds);
        SOCKET max_fd = socket.get_fd();

        timeval timeout{};
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;

        int activity = select(0, &readfds, nullptr, nullptr, &timeout);
        if (activity > 0 && FD_ISSET(socket.get_fd(), &readfds))
        {
            receivePacketAndPrint();
        }

        if (_kbhit())
        {
            cout << "[Client] : ";
            string input;
            getline(cin, input);

            if (input == "exit")
                break;
            sendPacketFromString(input, CommandType::HOST_INFO_RESPONSE);
        }

        this_thread::sleep_for(chrono::milliseconds(50));
    }
}
