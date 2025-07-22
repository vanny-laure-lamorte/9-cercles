#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "../include/LPTF_Socket.h"

TEST_CASE("Client socket creation", "[client]") {
    LPTF_Socket client;
    REQUIRE(client.create());
}

TEST_CASE("Client connect fails with bad IP", "[client]") {
    LPTF_Socket client;
    client.create();
    REQUIRE_FALSE(client.connect("256.256.256.256", 12345));
}

TEST_CASE("Client closes socket", "[client]") {
    LPTF_Socket client;
    REQUIRE(client.create());
    client.close();
    REQUIRE(client.get_fd() == INVALID_SOCKET);
}

TEST_CASE("Client sendMessage", "[client]") {
    LPTF_Socket client;
    REQUIRE(client.create());
    REQUIRE(client.connect("127.0.0.1", 123));
    REQUIRE(client.sendMsg("Should succeed", 16));
}