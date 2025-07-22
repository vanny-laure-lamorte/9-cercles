#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "../include/LPTF_Socket.h"
#include "../include/LPTF_Packet.h"

TEST_CASE("Client socket creation", "[client]")
{
    LPTF_Socket client;
    REQUIRE(client.createSocket());
}

TEST_CASE("Client connect fails with bad IP", "[client]")
{
    LPTF_Socket client;
    client.createSocket();
    REQUIRE_FALSE(client.connectToServer("256.256.256.256", 12345));
}

TEST_CASE("Client closes socket", "[client]")
{
    LPTF_Socket client;
    REQUIRE(client.createSocket());
    client.closeSocket();
    REQUIRE(client.getSocket() == INVALID_SOCKET);
}

TEST_CASE("Client sendMessage fails if not connected", "[client]")
{
    LPTF_Socket client;
    REQUIRE(client.createSocket());
    REQUIRE_FALSE(client.sendMessage("Should fail", strlen("Should fail")));
}

TEST_CASE("Client receiveMessage", "[client]")
{
    LPTF_Socket client;
    REQUIRE(client.createSocket());
    REQUIRE(client.connectToServer("127.0.0.1", 12345));

    std::string message = "Should succeed";
    std::vector<uint8_t> payload(message.begin(), message.end());
    LPTF_Packet packet(0x01, payload);
    std::vector<uint8_t> serialized = packet.serialize();
    REQUIRE(client.sendMessage(serialized.data(), serialized.size()));
    uint8_t buffer[1024];
    int received = client.receiveMessage(buffer, sizeof(buffer));
    REQUIRE(received >= 0);
    std::vector<uint8_t> data(buffer, buffer + received);
    LPTF_Packet response = LPTF_Packet::deserialize(data);
    REQUIRE(response.getPayload().size() > 0);
}
