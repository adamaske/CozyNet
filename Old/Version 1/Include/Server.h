#pragma once

#include "CozyNet.h"
#include "Utils/Constants.h"
#include "Utils/Endpoint.h"
#include "Utils/Socket.h"
#include "Utils/Connection.h"

#include <vector>

class Server {
private:
	//Connection
	std::vector<Connection> connections;

	// Listening Socket
	Socket listen_socket;
	WSAPOLLFD listen_fd;
	WSAPOLLFD polled_listen_fd;
public:
	int Initialize(const Endpoint& _endpoint);

	int Frame();

	int Poll();

	int AcceptConnections();
	int CreateConnection();

	int ServiceConnections();
	int ServiceConnection(const Connection& connection);

	void Listen();


	void OnConnect(const Connection& connection);
	void OnDisconnect();
protected:
	
};