#include "CozyNet.h"

#include "Utils/Constants.h"
#include <iostream>


int CozyNet::Initialize()
{
	WSADATA wsaData;
	int err = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (err != 0) {
		Error("WSA Startup Failed : " + std::to_string(err));
		Shutdown();
		return CN_ERROR;
	}

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {

		Error("WSA Found no useable Winsock DLL");
		Shutdown();
		return CN_ERROR;
	}

	Info("CozyNet Initialized");
	return CN_SUCCESS;
}

void CozyNet::Shutdown()
{
	WSACleanup();
	Info("CozyNet Shutdown");
}

void CozyNet::Info(std::string message)
{
	if (InfoCallback) {
		InfoCallback(message);
		return;
	}
	
	std::cout << message << "\n";
}

void CozyNet::Error(std::string message)
{
	if (ErrorCallback) {
		ErrorCallback(message);
		return;
	}

	std::cout << message << "\n";
}

void CozyNet::SetInfoCallback(LogCallbackParameter callback)
{
	InfoCallback = callback;
}

void CozyNet::SetErrorCallback(LogCallbackParameter callback)
{
	ErrorCallback = callback;
}