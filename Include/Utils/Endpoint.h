#pragma once

#include "CozyNet.h"

#include <string>
#include <vector>

class Endpoint {
private:
	std::string hostname = "";
	std::string ip_string = "";
	std::vector<uint8_t> ip_bytes = {};
	short port = 0;
public:
	Endpoint(std::string _ip, short _port);
	Endpoint(sockaddr* addr);

	std::string ToString();
};