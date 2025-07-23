#ifndef LPTF_PACKET_HPP
#define LPTF_PACKET_HPP

#include <cstdint>
#include <vector>
#include <stdexcept>
#include <memory>
#include <cstring>
#include <iostream>
using namespace std;

class LPTF_Packet
{
public:
    // Constructor
    LPTF_Packet(uint8_t type, const vector<uint8_t> &payload);
    // Destructor
    ~LPTF_Packet() = default;

    // Copy constructor
    LPTF_Packet(const LPTF_Packet &other);
    // Assignment operator
    LPTF_Packet &operator=(const LPTF_Packet &other);

    // Serialize the packet to a byte vector
    vector<uint8_t> serialize() const;

    // Deserialize a byte vector to create a packet
    static LPTF_Packet deserialize(const vector<uint8_t> &data);

    // Getters for packet properties
    uint8_t getVersion() const;
    uint8_t getType() const;
    uint16_t getLength() const;
    const vector<uint8_t> &getPayload() const;

private:
    // Packet properties
    uint8_t version;
    uint8_t type;
    uint16_t length;
    vector<uint8_t> payload;
};

#endif // LPTF_PACKET_HPP