#include <catch2/catch_test_macros.hpp>
#include "../include/lptf_socket.h"

TEST_CASE("Server socket creation", "[server]") {
    LPTF_Socket server;
    REQUIRE(server.createSocket());
}

TEST_CASE("Server bind fails on invalid IP", "[server]") {
    LPTF_Socket server;
    REQUIRE(server.createSocket());
    REQUIRE_FALSE(server.bindSocket("999.999.999.999", 12345));
}

TEST_CASE("Server bind + listen", "[server]") {
    LPTF_Socket server;
    REQUIRE(server.createSocket());
    REQUIRE(server.bindSocket("127.0.0.1", 54321));
    REQUIRE(server.listenSocket(1));
}
