#include "Utils/Endpoint.h"
#include "Utils/Constants.h"

#define WIN32_LEAN_AND_MEAN
#include "WinSock2.h"
#include <WS2tcpip.h>

#include "CozyNet.h"

Endpoint::Endpoint()
{
}

Endpoint::Endpoint(std::string _ip, short _port)
{
	port = _port;

	in_addr addr = {}; //inet_ "presentation format a.k.a "127.0..." to "network format"
	int result = inet_pton(AF_INET, _ip.c_str(), &addr);//
	if (result == 1) {
		if (addr.S_un.S_addr != INADDR_NONE) { // failed
			hostname = _ip;
			ip_string = _ip;
			//Ip version
			ip_bytes.resize(sizeof(ULONG));
			//We know that an IPv4 ip address is 4 bytes long, a ULONG is 4 bytes, thus we can copy
			memcpy(&ip_bytes[0], &addr.S_un.S_addr, sizeof(ULONG));
			return;
		}
	}

	//Resolve hostname
	addrinfo hints = {};//Filter for ipv4 only
	hints.ai_family = AF_INET; // only ipv4 only
	addrinfo* hostinfo = nullptr; //Pointer to linked list
	result = getaddrinfo(_ip.c_str(), NULL, &hints, &hostinfo);
	if (result != 0) {
		CozyNet::Error("Could not resolve hostname to IPv4 : " + std::to_string(WSAGetLastError()));
		return;
	}
	sockaddr_in* host_addr = reinterpret_cast<sockaddr_in*>(hostinfo->ai_addr);
	hostname = _ip;
	ip_string.resize(16);
	inet_ntop(AF_INET, &host_addr->sin_addr, &ip_string[0], 16);

	ULONG ip_long = host_addr->sin_addr.S_un.S_addr;
	ip_bytes.resize(sizeof(ULONG));
	memcpy(&ip_bytes[0], &ip_long, sizeof(ULONG));

	freeaddrinfo(hostinfo); // free linked list
}

Endpoint::Endpoint(sockaddr* addr) {
	sockaddr_in* _addr = reinterpret_cast<sockaddr_in*>(addr);
	port = _addr->sin_port;//ntohs(_addr->sin_port);//little to big endian
	ip_bytes.resize(sizeof(ULONG));
	memcpy(&ip_bytes[0], &_addr->sin_addr, sizeof(ULONG));
	ip_string.resize(16);
	inet_ntop(AF_INET, &_addr->sin_addr, &ip_string[0], 16);
	hostname = ip_string;
}

sockaddr_in Endpoint::GetSockaddr()
{
	sockaddr_in addr = {};
	addr.sin_family = AF_INET;
	memcpy(&addr.sin_addr.S_un.S_addr, &ip_bytes[0], sizeof(ULONG)); //Populate the addres with our ip_bytes
	addr.sin_port = htons(port);//swap endianess, host to network-byte-order short
	return addr;
}

std::string Endpoint::ToString() {
	//Something wierd is added on the end of the ip_string during inet_ntop making imgui unable to read it, thus c_str conversion
	return "[" + ip_string + " : " + std::to_string(port) + "]";
}