#include "LPTF_Packet.h"

using namespace std;

LPTF_Packet::LPTF_Packet(uint8_t type, const vector<uint8_t> &payload)
    : version(1), type(type), length(static_cast<uint16_t>(payload.size())), payload(payload) {}

LPTF_Packet::LPTF_Packet(const LPTF_Packet &other)
    : version(other.version), type(other.type), length(other.length), payload(other.payload) {}

LPTF_Packet &LPTF_Packet::operator=(const LPTF_Packet &other)
{
    if (this != &other)
    {
        version = other.version;
        type = other.type;
        length = other.length;
        payload = other.payload;
    }
    return *this;
}

std::vector<uint8_t> LPTF_Packet::serialize() const
{
    std::vector<uint8_t> buffer;
    uint16_t totalSize = 2 + 1 + 1 + payload.size();
    buffer.push_back((totalSize >> 8) & 0xFF);
    buffer.push_back(totalSize & 0xFF);
    buffer.push_back(version);
    buffer.push_back(type);
    buffer.insert(buffer.end(), payload.begin(), payload.end());

    return buffer;
}

LPTF_Packet LPTF_Packet::deserialize(const std::vector<uint8_t> &data)
{
    if (data.size() < 4)
    {
        throw std::invalid_argument("Data too short");
    }
    uint16_t totalSize = (static_cast<uint16_t>(data[0]) << 8) | data[1];
    if (data.size() != totalSize)
    {
        throw std::invalid_argument("Packet size mismatch");
    }
    cout << "[PACKET SIZE] : " << static_cast<int>(totalSize) << endl;
    uint8_t version = data[2];
    cout << "[PACKET VERSION] : " << static_cast<int>(version) << endl;
    uint8_t type = data[3] ;
    cout << "[PACKET TYPE] : " << static_cast<int>(type) << endl;
    std::vector<uint8_t> payload(data.begin() + 4, data.end());
    cout << "[PAYLOAD SIZE] : " << payload.size() << endl;
    cout << "[PAYLOAD BYTES] : ";
    for (const auto &byte : payload)
    {
        cout << static_cast<int>(byte) << " ";
    } cout << endl;
    return LPTF_Packet(type, payload);
}

uint8_t LPTF_Packet::getVersion() const
{
    return version;
}

uint8_t LPTF_Packet::getType() const
{
    return type;
}

uint16_t LPTF_Packet::getLength() const
{
    return length;
}

const vector<uint8_t> &LPTF_Packet::getPayload() const
{
    return payload;
}
