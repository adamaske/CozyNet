#include "Utils/Socket.h"

Socket::Socket()
{
}

Socket::Socket(SocketHandle _socket)
{
	ip_socket = _socket;
}

int Socket::Create(bool blocking)
{
    ip_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(ip_socket == INVALID_SOCKET) {
		CozyNet::Error("Socket Creation Error" + std::to_string(WSAGetLastError()));
		return CN_ERROR;
	}

	int result = SetBlocking(blocking);
	if (result == CN_ERROR) {
		CozyNet::Error("SOCKET : Set Blocking Error" + std::to_string(WSAGetLastError()));
		return CN_ERROR;
	}

	result = SetSocketOption(SocketOption::TCP_NoDelay, TRUE);
	if (!result) {
		CozyNet::Error("SOCKET : Set NoDelay Error" + std::to_string(WSAGetLastError()));
		return CN_ERROR;
	}
	return CN_SUCCESS;
}

int Socket::Close()
{
	if (ip_socket == INVALID_SOCKET) {
		CozyNet::Error("SOCKET : Cannot close invalid socket");
		return CN_ERROR;
	}

	int result = closesocket(ip_socket);
	if (result != 0) {
		CozyNet::Error("SOCKET : Socket close error : " + std::to_string(WSAGetLastError()));
		return CN_ERROR;
	}

	return CN_SUCCESS;
}

int Socket::Bind(Endpoint endpoint)
{
	sockaddr_in addr = endpoint.GetSockaddr();

	int result = bind(ip_socket, (sockaddr*)&addr, sizeof(sockaddr_in));
	if (result != 0) {
		CozyNet::Error("SOCKET : Bind failed : " + WSAGetLastError());
		return CN_ERROR;
	}

	return CN_SUCCESS;
}

int Socket::Listen(Endpoint endpoint, int backlog)
{
	int bound = Bind(endpoint);
	if (bound == CN_ERROR) {
		CozyNet::Error("SOCKET : Listen failed as binding failed...");
		return CN_ERROR;
	}

	int result = listen(ip_socket, backlog);
	if (result != 0) {
		CozyNet::Error("SOCKET : Listen failed : " + std::to_string(WSAGetLastError()));
		return CN_ERROR;
	}

	return CN_SUCCESS;
}

int Socket::Accept(Socket* out_socket, Endpoint* out_endpoint)
{
	sockaddr_in addr = {};
	int len = sizeof(sockaddr_in);

	SocketHandle accepted = accept(ip_socket, (sockaddr*)&addr, &len); //Returns INVALID_SOCKET 
	if (accepted == INVALID_SOCKET) {
		CozyNet::Error("SOCKET : Accept failed : " + std::to_string(WSAGetLastError()));
		return CN_ERROR;
	}

	*out_socket = Socket(accepted);
	*out_endpoint = Endpoint((sockaddr*)&addr);
	return CN_SUCCESS;
}

int Socket::Connect(Endpoint endpoint)
{
	sockaddr_in addr = endpoint.GetSockaddr();
	int result = connect(ip_socket, (sockaddr*)&addr, sizeof(sockaddr_in));
	if (result != 0) {
		CozyNet::Error("SOCKET : Connection failed : " + std::to_string(WSAGetLastError()));
		return CN_ERROR;
	}

	return CN_SUCCESS;
}

int Socket::Send(const void* data, int byteAmount, int& bytesSent)
{
	bytesSent = send(ip_socket, (const char*)data, byteAmount, 0);
	if (bytesSent == INVALID_SOCKET) {
		CozyNet::Error("SOCKET : Sending error : " + std::to_string(WSAGetLastError()));
		return CN_ERROR;
	}
	return CN_SUCCESS;
}

int Socket::SendAll(const void* data, int byteAmount)
{
	int totalBytesSent = 0;
	while (totalBytesSent < byteAmount) {
		int bytesRemaining = byteAmount - totalBytesSent;
		int bytesSent = 0;

		char* buffer_offset = (char*)data + totalBytesSent;
		int result = Send(buffer_offset, bytesRemaining, bytesSent);
		if (result == CN_ERROR) {
			return CN_ERROR;
		}
		totalBytesSent += bytesSent;
	}

	return CN_SUCCESS;
}

int Socket::Recv(void* destination, int byteAmount, int& bytesRecieved)
{
	bytesRecieved = recv(ip_socket, (char*)destination, byteAmount, NULL);

	if (bytesRecieved == 0) {
		//Gracefully closed
		CozyNet::Error("SOCKET : Recv : 0 bytes received = closed gracefully...");
		return CN_ERROR;
	}

	if (bytesRecieved == SOCKET_ERROR) {
		CozyNet::Error("SOCKET : Recieve error : " + std::to_string(WSAGetLastError()));
		return CN_ERROR;
	}

	return CN_SUCCESS;
}

int Socket::RecvAll(void* destination, int byteAmount)
{
	int totalBytesReceived = 0;
	while (totalBytesReceived < byteAmount) {
		int bytesRemaining = byteAmount - totalBytesReceived;
		int bytesReceived = 0;

		char* buffer_offset = (char*)destination + totalBytesReceived;
		int result = Recv(buffer_offset, bytesRemaining, bytesReceived);
		if (result == CN_ERROR) {
			CozyNet::Error("SOCKET : RecvAll : Error");
			return CN_ERROR;
		}
		totalBytesReceived += bytesReceived;
	}

	return CN_SUCCESS;
}

int Socket::Send(Packet& packet)
{
	uint16_t encoded_packet_size = htons(packet.buffer.size());

	int result = SendAll(&encoded_packet_size, sizeof(uint16_t));
	if (result == CN_ERROR) {
		return CN_ERROR;
	}

	result = SendAll(packet.buffer.data(), packet.buffer.size());
	if (result == CN_ERROR) {
		return CN_ERROR;
	}

	return CN_SUCCESS;
}

int Socket::Recv(Packet& packet)
{
	packet.Clear();

	uint16_t encoded_size = 0;
	int result = RecvAll(&encoded_size, sizeof(uint16_t));
	if (result == CN_ERROR) {
		return CN_ERROR;
	}

	uint16_t buffer_size = ntohs(encoded_size);

	if (buffer_size > MAX_PACKET_SIZE) {
		CozyNet::Error("SOCKET : Recv buffer size exeeded max_packet : " + std::to_string(buffer_size));
		return CN_ERROR;
	}

	packet.buffer.resize(buffer_size);
	result = RecvAll(&packet.buffer[0], buffer_size);
	if (result == CN_ERROR) {
		return CN_ERROR;
	}

	return CN_SUCCESS;
}

int Socket::SetBlocking(bool blocking)
{
	unsigned long no_block = 1;
	unsigned long block = 0;
	int result = ioctlsocket(ip_socket, FIONBIO, blocking ? &block : &no_block);
	if (result == SOCKET_ERROR) {
		CozyNet::Error("SOCKET : Failed seting blocking mode : " + WSAGetLastError());
		return CN_ERROR;
	}
	return CN_SUCCESS;
}

int Socket::SetSocketOption(SocketOption opt, BOOL value)
{
	int result = 0;
	switch (opt) {
	case TCP_NoDelay:
		result = setsockopt(ip_socket, IPPROTO_TCP, TCP_NODELAY, (const char*)&value, sizeof(value));
		break;
	default:
		break;
	}

	if (result != 0) {
		CozyNet::Error("SOCKET : Set Socket Option failed with error : " + WSAGetLastError());
		return CN_ERROR;
	}

	return CN_SUCCESS;
}
