#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "../include/LPTF_Socket.h"
#include "../include/LPTF_Packet.h"

TEST_CASE("Client socket creation", "[client]")
{
    LPTF_Socket client;
    REQUIRE(client.create());
}

TEST_CASE("Client connect fails with bad IP", "[client]")
{
    LPTF_Socket client;
    client.create();
    REQUIRE_FALSE(client.connect("256.256.256.256", 12345));
}

TEST_CASE("Client closes socket", "[client]")
{
    LPTF_Socket client;
    REQUIRE(client.create());
    client.close();
    REQUIRE(client.get_fd() == INVALID_SOCKET);
}

TEST_CASE("Client sendMessage fails if not connected", "[client]")
{
    LPTF_Socket client;
    REQUIRE(client.create());
    REQUIRE_FALSE(client.sendMsg("Should fail", strlen("Should fail")));
}

TEST_CASE("Client receiveMessage", "[client]")
{
    LPTF_Socket client;
    REQUIRE(client.create());
    REQUIRE(client.connect("127.0.0.1", 12345));

    std::string message = "Should succeed";
    std::vector<uint8_t> payload(message.begin(), message.end());
    LPTF_Packet packet(0x01, payload);
    std::vector<uint8_t> serialized = packet.serialize();
    REQUIRE(client.sendMsg(serialized.data(), serialized.size()));

    uint8_t buffer[1024];
    int received = client.receivePacket(buffer, sizeof(buffer));
    REQUIRE(received >= 0);

    std::vector<uint8_t> data(buffer, buffer + received);
    LPTF_Packet response = LPTF_Packet::deserialize(data);
    REQUIRE(response.getPayload().size() > 0);
}