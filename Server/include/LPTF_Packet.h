#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <stdexcept>

class LPTF_Packet {
private:
    uint8_t type_;
    std::vector<uint8_t> payload_;

public:
    LPTF_Packet();
    LPTF_Packet(uint8_t type, const std::vector<uint8_t>& payload);
    LPTF_Packet(uint8_t type, const std::string& payload);

    uint8_t getType() const;
    const std::vector<uint8_t>& getPayload() const;
    void setType(uint8_t type);
    void setPayload(const std::vector<uint8_t>& payload);

    std::vector<uint8_t> serialize() const;
    static LPTF_Packet deserialize(const uint8_t* data, size_t size, size_t& consumedBytes);

    std::string getPayloadAsString() const;
};
