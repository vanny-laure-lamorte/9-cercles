#include "../include/LPTF_Packet.h"

LPTF_Packet::LPTF_Packet(uint8_t type, const vector<uint8_t>& payload)
    : version(1), type(type), length(static_cast<uint16_t>(payload.size())), payload(payload) {}

LPTF_Packet::LPTF_Packet(const LPTF_Packet& other)
    : version(other.version), type(other.type), length(other.length), payload(other.payload) {}

LPTF_Packet& LPTF_Packet::operator=(const LPTF_Packet& other) {
    if (this != &other) {
        version = other.version;
        type = other.type;
        length = other.length;
        payload = other.payload;
    }
    return *this;
}

vector<uint8_t> LPTF_Packet::serialize() const {
    vector<uint8_t> buffer;
    buffer.reserve(4 + payload.size());

    buffer.push_back(version);
    buffer.push_back(type);
    buffer.push_back(static_cast<uint8_t>((length >> 8) & 0xFF));
    buffer.push_back(static_cast<uint8_t>(length & 0xFF));

    buffer.insert(buffer.end(), payload.begin(), payload.end());
    return buffer;
}

LPTF_Packet LPTF_Packet::deserialize(const vector<uint8_t>& data) {
    if (data.size() < 4) {
        throw invalid_argument("Data too short to be a valid packet");
    }

    uint8_t version = data[0];
    uint8_t type = data[1];
    uint16_t length = (static_cast<uint16_t>(data[2]) << 8) | data[3];

    if (data.size() < 4 + length) {
        throw invalid_argument("Payload length mismatch");
    }

    vector<uint8_t> payload(data.begin() + 4, data.begin() + 4 + length);
    return LPTF_Packet(type, payload);
}

uint8_t LPTF_Packet::getVersion() const {
    return version;
}

uint8_t LPTF_Packet::getType() const {
    return type;
}

uint16_t LPTF_Packet::getLength() const {
    return length;
}

const vector<uint8_t>& LPTF_Packet::getPayload() const {
    return payload;
}
