#include "../include/LPTF_Packet.h"
#include <winsock2.h>

LPTF_Packet::LPTF_Packet()
    : type_(0), payload_()
{}

LPTF_Packet::LPTF_Packet(uint8_t type, const std::vector<uint8_t>& payload)
    : type_(type), payload_(payload)
{}

LPTF_Packet::LPTF_Packet(uint8_t type, const std::string& payload)
    : type_(type), payload_(payload.begin(), payload.end())
{}

uint8_t LPTF_Packet::getType() const {
    return type_;
}

const std::vector<uint8_t>& LPTF_Packet::getPayload() const {
    return payload_;
}

void LPTF_Packet::setType(uint8_t type) {
    type_ = type;
}

void LPTF_Packet::setPayload(const std::vector<uint8_t>& payload) {
    payload_ = payload;
}

std::vector<uint8_t> LPTF_Packet::serialize() const {
    if (payload_.size() > 0xFFFF) {
        throw std::length_error("Payload too large (> 65535 bytes)");
    }

    std::vector<uint8_t> buffer;
    buffer.reserve(1 + 2 + payload_.size());

    buffer.push_back(type_);

    uint16_t sizeBE = htons(static_cast<uint16_t>(payload_.size()));
    buffer.push_back(static_cast<uint8_t>((sizeBE >> 8) & 0xFF));
    buffer.push_back(static_cast<uint8_t>(sizeBE & 0xFF));

    buffer.insert(buffer.end(), payload_.begin(), payload_.end());

    return buffer;
}

LPTF_Packet LPTF_Packet::deserialize(const uint8_t* data, size_t size, size_t& consumedBytes) {
    if (size < 4) {
        throw std::runtime_error("Buffer too small to contain packet header");
    }

    uint8_t type = data[0];
    uint16_t payloadSize = ntohs(*reinterpret_cast<const uint16_t*>(data + 1));

    if (size < 4  + payloadSize) {
        throw std::runtime_error("Buffer too small to contain full payload");
    }

    std::vector<uint8_t> payload(data + 4, data + 4 + payloadSize);

    consumedBytes = 3 + payloadSize;
    return LPTF_Packet(type, payload);
}

std::string LPTF_Packet::getPayloadAsString() const {
    return std::string(payload_.begin(), payload_.end());
}

