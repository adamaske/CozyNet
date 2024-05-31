#include <iostream>
#include <string>

#include "CozyNet.h"
#include "Server.h"
#include "Client.h"

#include "Utils/Constants.h"
#include "Utils/Endpoint.h"
#include "Utils/Socket.h"
#include "Utils/Packet.h"
#include "Utils/Connection.h"


void InfoCallback(std::string info) {
    std::cout << "INFO : " << info << "\n";
}
void ErrorCallback(std::string error) {
    std::cout << "ERROR : " << error << "\n";
}

int main(int argc, char* argv[]) {

    CozyNet::SetInfoCallback(InfoCallback);
    CozyNet::SetErrorCallback(ErrorCallback);

    CozyNet::Initialize();

    Endpoint endpoint = Endpoint("127.0.0.1", 8400);
    Socket socket;

    Connection connection = Connection(socket, endpoint);
    Packet packet;

    CozyNet::Shutdown();

    return 0;
}