#include <catch2/catch_test_macros.hpp>
#include "../include/LPTF_Socket.h"

TEST_CASE("Server socket creation", "[server]") {
    LPTF_Socket server;
    REQUIRE(server.create());
}

TEST_CASE("Server bind fails if already bound to same port", "[server]") {
    LPTF_Socket s1, s2;
    REQUIRE(s1.create());
    REQUIRE(s2.create());
    REQUIRE(s1.bind(4567));
    REQUIRE_FALSE(s2.bind(4567));
}

TEST_CASE("Server bind + listen", "[server]") {
    LPTF_Socket server;
    REQUIRE(server.create());
    REQUIRE(server.bind(123));
    REQUIRE(server.listen());
}