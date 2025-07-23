#include <catch2/catch_test_macros.hpp>
#include "../include/lptf_socket.h"

TEST_CASE("Server socket creation", "[server]")
{
    LPTF_Socket server;
    REQUIRE(server.create());
}

TEST_CASE("Server binds on an already used port", "[server]")
{
    LPTF_Socket server;
    REQUIRE(server.create());
    REQUIRE(server.bind(123));
    REQUIRE_FALSE(server.bind(123));
}

TEST_CASE("Server bind + listen", "[server]")
{
    LPTF_Socket server;
    REQUIRE(server.create());
    REQUIRE(server.bind(54321));
    REQUIRE(server.listen());
}

