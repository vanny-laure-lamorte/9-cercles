#ifndef LPTF_PACKET_HPP
#define LPTF_PACKET_HPP
#include <cstring>
#include <cstdint>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>
using namespace std;

/**
 * LPTF_Packet
 * Represents a packet in the LPTF protocol.
 * It contains a type, version, length, and payload.
 */
class LPTF_Packet
{
public:
    // Constructor: Initializes a packet with type and payload
    LPTF_Packet(uint8_t type, const vector<uint8_t> &payload);
    // Destructor
    ~LPTF_Packet() = default;

    // Copy constructor and assignment operator
    LPTF_Packet(const LPTF_Packet &other);
    // Assignment operator
    LPTF_Packet &operator=(const LPTF_Packet &other);

    // Serializes the packet into a byte vector
    vector<uint8_t> serialize() const;
    // Deserializes a byte vector into a packet
    static LPTF_Packet deserialize(const vector<uint8_t> &data);

    // Getters for packet properties
    uint8_t getVersion() const;
    uint8_t getType() const;
    uint16_t getLength() const;
    const vector<uint8_t> &getPayload() const;

private:
    // Packet version, type, length, and payload
    uint8_t version;
    uint8_t type;
    uint16_t length;
    vector<uint8_t> payload;
};
#endif // LPTF_PACKET_H