#include <catch2/catch_test_macros.hpp>
#include "../include/LPTF_Packet.h"

TEST_CASE("Packet serialization and deserialization", "[packet]") {
    std::vector<uint8_t> payload = {'H', 'e', 'l', 'l', 'o'};
    uint8_t type = 0x42;

    LPTF_Packet packet(type, payload);
    std::vector<uint8_t> serialized = packet.serialize();

    REQUIRE(serialized.size() == payload.size() + 2);

    LPTF_Packet deserialized = LPTF_Packet::deserialize(serialized);

    REQUIRE(deserialized.getType() == type);
    REQUIRE(deserialized.getPayload() == payload);
}
