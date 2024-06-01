#include "Client.h"

#include "CozyNet.h"
Client::Client()
{
}
int Client::Connect(const Endpoint& _endpoint)
{
    endpoint = _endpoint;
    connected = false;

    int result = socket.Create();
    if (result == CN_ERROR) {
        return CN_ERROR;
    }

    socket.SetBlocking(true);

    result = socket.Connect(endpoint);
    if (result == CN_ERROR) {
        return CN_ERROR;
    }

    connected = true;
    return CN_SUCCESS;
}

int Client::Close()
{
    if (!connected) {
        return CN_SUCCESS;
    }

    int result = socket.Close();
    if (result == CN_ERROR) {
        return CN_ERROR;
    }

    connected = false;
    return CN_SUCCESS;
}

int Client::Frame()
{
    if (!connected) {
        return CN_SUCCESS;
    }

    //Read 
    int result = HandleIncomingPackets();
    if (result == CN_ERROR) {
        //Something is wrong, close connection
        return CN_ERROR;
    }

    result = HandleOutgoingPackets();
    if (result == CN_ERROR) {
        //Something is wrong, close connection
        return CN_ERROR;
    }

    return CN_SUCCESS;
}

int Client::HandleIncomingPackets()
{
    while (!incoming_packets.empty()) {

        auto front = incoming_packets.front();

        ProcessPacket(front);
    }
    return CN_SUCCESS;
}

int Client::HandleOutgoingPackets()
{
    while (!outgoing_packets.empty()) {

        auto front = outgoing_packets.front();

        socket.Send(front);
    }
    return CN_SUCCESS;
}

int Client::ProcessPacket(Packet packet)
{
    //
    return CN_SUCCESS;
}

bool Client::IsConnected()
{
    return connected;
}

Endpoint Client::GetEndpoint()
{
    return endpoint;
}
