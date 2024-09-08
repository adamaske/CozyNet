#pragma once

#include "CozyNet.h"

#include "Utils/Socket.h"
#include "Utils/Endpoint.h"

class Connection {
private:

public:
	Connection(Socket _socket, Endpoint _endpoint);

	Endpoint endpoint;
	Socket socket;
	WSAPOLLFD fd;
	WSAPOLLFD polled_fd;
};