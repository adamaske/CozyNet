#include "Utils/Connection.h"
Connection::Connection(Socket _socket, Endpoint _endpoint) 
	: endpoint(_endpoint), socket(_socket)
{

}
