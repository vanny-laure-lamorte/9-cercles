#ifndef LPTF_PACKET_HPP
#define LPTF_PACKET_HPP

#include <cstdint>
#include <vector>
#include <stdexcept>
#include <memory>
#include <cstring>
using namespace std;

class LPTF_Packet {
public:
    LPTF_Packet(uint8_t type, const vector<uint8_t>& payload);
    LPTF_Packet(const LPTF_Packet& other);
    LPTF_Packet& operator=(const LPTF_Packet& other);
    ~LPTF_Packet() = default;

    vector<uint8_t> serialize() const;
    static LPTF_Packet deserialize(const vector<uint8_t>& data);

    uint8_t getVersion() const;
    uint8_t getType() const;
    uint16_t getLength() const;
    const vector<uint8_t>& getPayload() const;

private:
    uint8_t version;
    uint8_t type;
    uint16_t length;
    vector<uint8_t> payload;
};
#endif // LPTF_PACKET_HPP