#include <iostream>
#include <string>
#include <thread>

#include "CozyNet.h"
#include "Server.h"
#include "Client.h"

#include "Utils/Constants.h"
#include "Utils/Endpoint.h"
#include "Utils/Socket.h"
#include "Utils/Packet.h"
#include "Utils/Packet2.h"
#include "Utils/Connection.h"


void InfoCallback(std::string info) {
    std::cout << "INFO : " << info << "\n";
}
void ErrorCallback(std::string error) {
    std::cout << "ERROR : " << error << "\n";
}

void RunClient(Endpoint endpoint, bool* should_run) {

    Client client;
    client.Connect(endpoint);

    while (client.IsConnected() && should_run) {
        client.Frame();
    }
}
int main(int argc, char* argv[]) {

    CozyNet::SetInfoCallback(InfoCallback);
    CozyNet::SetErrorCallback(ErrorCallback);

    CozyNet::Initialize();

    
    Endpoint endpoint = Endpoint("127.0.0.1", 8400);

    Server server;
    server.Initialize(endpoint);
    bool run_server = true;
    bool run_client = true;

    std::thread client_thread(RunClient, endpoint, &run_client);
    
    while (run_server) {

        server.Frame();

    }
    
    client_thread.join();
    CozyNet::Shutdown();

    return 0;
}