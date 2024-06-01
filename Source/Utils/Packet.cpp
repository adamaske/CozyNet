#include "Utils/Packet.h"


Packet::Packet(PacketType type)
{
	Clear(); //Clear everything, set invalid, set extraction offset
	AssignPacketType(type); //set type
}

void Packet::AssignPacketType(PacketType packet_type) {
	//Look at the first 4 bytes as a packettype
	PacketType* packet_type_ptr = reinterpret_cast<PacketType*>(&buffer[0]);
	//Value of the pointer is PacketConvert to network-byte-order
	*packet_type_ptr = static_cast<PacketType>(htonl(packet_type));
	extraction_offset = sizeof(PacketType); // we dont want to extract from here
}

PacketType Packet::GetPacketType() {
	//Look at the address of 0th buffer element as a pointer to a packet type (uint32_t)
	PacketType* packet_type_ptr = reinterpret_cast<PacketType*>(&buffer[0]); 
	//Convert to host-byte-order
	return static_cast<PacketType>(ntohl(*packet_type_ptr));
}

void Packet::Clear() {
	buffer.resize(sizeof(PacketType)); // 4 bytes
	AssignPacketType(Invalid);
}

void Packet::Append(const void* data, uint32_t size) { //
	if (buffer.size() + size > MAX_PACKET_SIZE) {
		CozyNet::Error("Packet : MAX_PACKET_SIZE exceeded");
		return;
	}
	buffer.insert(buffer.end(), (char*)data, (char*)data + size);
}

Packet& Packet::operator << (uint32_t data) { //Insert an integer
	data = htonl(data); //Store as network-order
	Append(&data, sizeof(uint32_t)); //Add
	return *this;
}

Packet& Packet::operator >> (uint32_t& data) {
	if ((extraction_offset + sizeof(uint32_t)) > buffer.size()) {
		CozyNet::Error("Packet : Extraction_offset exceeded buffer size");
	}
	data = *reinterpret_cast<uint32_t*>(&buffer[extraction_offset]);// derefrence Look at buffer[e_o] as a uint32_t-poiner
	data = ntohl(data); //this 

	extraction_offset += sizeof(uint32_t); //4 + 4 = 8 -> 
	return *this;
}

Packet& Packet::operator << (const std::string& data) {
	*this << (uint32_t)data.size();
	Append(data.data(), data.size());
	return *this;
}

Packet& Packet::operator >> (std::string& data) {

	data.clear(); //empty the string

	uint32_t string_size = 0; 
	*this >> string_size; //The first 4 bytes of a string is its size

	if ((extraction_offset + string_size) > buffer.size()) {
		CozyNet::Error("Packet : Extraction_offset exceeded buffer size");
	}

	data.resize(string_size);
	data.assign(&buffer[extraction_offset], string_size); //Start at extraction offset (should be 8), go for string size

	extraction_offset += string_size;
	return *this;
}