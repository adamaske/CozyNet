#pragma once

#include "CozyNet.h"
#include "Utils/Endpoint.h"
#include "Utils/Packet.h"

enum SocketOption { TCP_NoDelay };
typedef SOCKET SocketHandle;
class Socket {
private:
	SocketHandle ip_socket;

public:
	Socket();
	Socket(SocketHandle _socket);

	int Create(bool blocking = true);
	int Close();

	int Bind(Endpoint endpoint);
	int Listen(Endpoint endpoint, int backlog = 5);
	int Accept(Socket* out_socket, Endpoint* out_endpoint);

	int Connect(Endpoint endpoint);

	int Send(const void* data, int byteAmount, int& bytesSent);
	int SendAll(const void* data, int byteAmount);

	int Recv(void* destination, int byteAmount, int& bytesRecieved);
	int RecvAll(void* destination, int byteAmount);

	int Send(Packet& packet);
	int Recv(Packet& packet);

	int SetBlocking(bool blocking);
	int SetSocketOption(SocketOption opt, BOOL value);

	SocketHandle GetSocket() {
		return ip_socket;
	}
};