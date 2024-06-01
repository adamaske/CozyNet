#include "Server.h"

int Server::Initialize(const Endpoint& _endpoint)
{
	Endpoint endpoint = _endpoint;
	listen_socket = Socket();

	int result = listen_socket.Create(false);
	if (result == CN_ERROR) {
		CozyNet::Error("Server : Listen_socket failed creation");
		return CN_ERROR;
	}

	result = listen_socket.Listen(endpoint);
	if (result == CN_ERROR) {
		CozyNet::Error("Server : Failed listening... ");
		return CN_ERROR;
	}

	listen_fd = {};
	listen_fd.fd = listen_socket.GetSocket();
	listen_fd.events = POLLRDNORM; //Listening only cares about reading
	listen_fd.revents = 0; //

	CozyNet::Info("Server : " + endpoint.ToString() + " - Listening.");

	return CN_SUCCESS;
}

int Server::Frame()
{
	int result = Poll();
	 
	return CN_SUCCESS;
}

int Server::Poll()
{ 
	//listen_fd, con1, con2, con3, ..., con(N)
	std::vector<WSAPOLLFD> fds = { listen_fd };
	for (auto& connection : connections) {
		fds.push_back(connection.fd);
	}

	int polls = WSAPoll(fds.data(), fds.size(), 1); //0th = listen, 1 to Size = connection, 
	if (polls == SOCKET_ERROR) {
		CozyNet::Error("Server : Poll error : " + std::to_string(WSAGetLastError()));
		return CN_ERROR;
	}

	//listen_fd, con1, con2, con3, ..., con(N)
	polled_listen_fd = fds[0];

	//Distrubtue polled fds to connections
	for (int i = 1; i < fds.size(); i++) { 
		connections[i].polled_fd = fds[i];
	}

	return CN_SUCCESS;
}

int Server::AcceptConnections()
{
	if (!polled_listen_fd.revents & POLLRDNORM) {
		return CN_SUCCESS;
	}

	Endpoint connected_endpoint;
	Socket connected_socket;

	int result = listen_socket.Accept(&connected_socket, &connected_endpoint);
	if (result == CN_ERROR) {
		CozyNet::Error("Server : Failed accept");
		return CN_ERROR;
	}

	

	WSAPOLLFD accepted_fd = {}; //Create read and write fd for this connection
	accepted_fd.fd = connected_socket.GetSocket();
	accepted_fd.events = POLLRDNORM | POLLWRNORM;
	accepted_fd.revents = 0;


	Connection connection = Connection(connected_socket, connected_endpoint);
	connection.fd = accepted_fd;
	connections.push_back(connection);
	OnConnect(connection);

	return CN_SUCCESS;
}

int Server::CreateConnection()
{
	return CN_SUCCESS;
}

int Server::ServiceConnections()
{
	for (auto& connection : connections) {
		int result = ServiceConnetion(connection);
		if (result == CN_ERROR) {

		}
	}
	return CN_SUCCESS;
}

int Server::ServiceConnetion(Connection& connection)
{
	auto& fd = connection.polled_fd;



	return CN_SUCCESS;
}

void Server::Listen()
{
}

void Server::OnConnect(const Connection& connection)
{
}

void Server::OnDisconnect()
{
}
