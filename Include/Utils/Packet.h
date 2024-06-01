#pragma once

#include "CozyNet.h"
#include <vector>
#include <string>

enum PacketType {
	Invalid,
	String,
	IntegerArray,
	Test,
};
//A Packet is an array of bytes
//4 bytes - PacketType
//
class Packet {
public:
	uint32_t extraction_offset = sizeof(PacketType); //Byte reading counter
	std::vector<char> buffer = {}; //Payload

	Packet(PacketType type = PacketType::Invalid);

	//Writes first 4 bytes of buffer as reintrerpeted PacketType
	void AssignPacketType(PacketType packet_type);
	//Reads first 4 bytes of buffer and reinterprets as PacketType
	PacketType GetPacketType();
	
	//Clear buffer, set packettype to invalid
	void Clear();
	
	//Add
	void Append(const void* data, uint32_t size);

	Packet& operator << (uint32_t data);
	Packet& operator >> (uint32_t& data);

	Packet& operator << (const std::string& data);
	Packet& operator >> (std::string& data);
};

//A packet is a wrapper for an array of bytes
