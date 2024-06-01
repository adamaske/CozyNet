#pragma once

#include "Utils/Endpoint.h"
#include "Utils/Socket.h"
#include "Utils/Packet.h"
#include <queue>

class Client {
private:
	Endpoint endpoint;
	Socket socket;
	bool connected = false;

	std::queue<Packet> incoming_packets;
	std::queue<Packet> outgoing_packets;
public:
	Client();

	int Connect(const Endpoint& _endpoint);
	int Close();
	int Frame();

	int HandleIncomingPackets();
	int HandleOutgoingPackets();

	int ProcessPacket(Packet packet);

	bool IsConnected();
	Endpoint GetEndpoint();
};