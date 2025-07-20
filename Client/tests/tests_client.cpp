#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "../include/LPTF_Socket.h"

TEST_CASE("Client socket creation", "[client]") {
    LPTF_Socket client;
    REQUIRE(client.createSocket());
}

TEST_CASE("Client connect fails with bad IP", "[client]") {
    LPTF_Socket client;
    client.createSocket();
    REQUIRE_FALSE(client.connectToServer("256.256.256.256", 12345));
}

TEST_CASE("Client closes socket", "[client]") {
    LPTF_Socket client;
    REQUIRE(client.createSocket());
    client.closeSocket();
    REQUIRE(client.getSocket() == INVALID_SOCKET);
}

TEST_CASE("Client sendMessage fails if not connected", "[client]") {
    LPTF_Socket client;
    REQUIRE(client.createSocket());
    REQUIRE_FALSE(client.sendMessage("Should fail"));
}

TEST_CASE("Client sendMessage", "[client]") {
    LPTF_Socket client;
    REQUIRE(client.createSocket());
    REQUIRE(client.connectToServer("127.0.0.1", 12345));
    REQUIRE(client.sendMessage("Should succeed"));
}
