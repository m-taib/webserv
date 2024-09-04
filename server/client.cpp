#include "../includes/client.hpp"

Client::Client(int sc) 
{
	_socket = sc;
	_init_time = -1;
	request = "";
}

Request&         Client::getRequest() 
{
	return _request;
}

Response&         Client::getResObj()
{
	return _response;
}



int      Client::getSocketFd() const
{
	return _socket;
}

void    Client::setResObj(Response response)
{
	_response = response;
}

