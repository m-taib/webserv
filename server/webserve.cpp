#include "../includes/webserve.hpp"
#include "../includes/request.hpp"
#include "../includes/response.hpp"
#include <_types/_intmax_t.h>
#include <codecvt>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <string_view>
#include <sys/_types/_fd_def.h>
#include <sys/_types/_size_t.h>
#include <sys/_types/_timeval.h>
#include <sys/fcntl.h>
#include <vector>
#include "../includes/response.hpp"



int      Server::_max_fd = 0;

Server::Server()
{

}
Server::Server(std::string filename)
{
	std::fstream    file;
    std::string     line;
	std::string 	token;
	
    file.open(filename.c_str(), std::ios::in);
    if (file.is_open())
    {
        while (getline(file, line))
        {
			if (line == "server")
			{
				_config_vec.push_back(ServerConfig(file));
				_config_vec.back().extract_data();
				// exit(0);
			}
			lunchServer();
			bindSocket();
		}
	}
	file.close();
	Response::initializeMap("configs/mime.types");
	_max_buff = 1024;
	buffer_size = 1024;
	_body_size = 0;
}

int      Server::getSocketFd() const
{
	return _socket_fd;
}

int      Server::getPort() const
{
	return _port;
}

long	get_time(void)
{
	struct timeval tv;

	if (gettimeofday(&tv, NULL) == -1) 
	{
		perror("gettimeofday");
		return 1;
	}
	
	// Extract seconds, discarding microseconds
	return (tv.tv_sec);
}



std::string    Server::getServerAddr() const
{
	return _addr;
}

void	Server::lunchServer()
{
	_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket_fd == -1)
	{
		perror("An error is occured\n");
		exit(1);
	}
	// int status = fcntl(_socket_fd, F_SETFL,  O_NONBLOCK);

	// if (status == -1)
	// {
	// 	perror("calling fcntl");
	// 	exit(EXIT_FAILURE);        
	// }
	// set socket options 
	int	reuse_addr = 1;

	if (setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof(reuse_addr)) == -1) 
	{
		perror("setsockopt");
		exit(1);
  	}
	std::cout << "Server socket : " << _socket_fd << std::endl;
	if (_socket_fd > _max_fd)
		_max_fd = _socket_fd;
	fds.push_back(_socket_fd);
}

void	Server::bindSocket()
{
	struct sockaddr_in server_adress;
	DIRS_PAIR dirs;
	std::string		value;
	std::string		addr;
	int		port;

	memset((char *)&server_adress, 0, sizeof(server_adress)); 
	server_adress.sin_family = AF_INET; 
	server_adress.sin_addr.s_addr = INADDR_ANY;
	// if (inet_pton(AF_INET, addr.c_str(), &server_adress.sin_addr.s_addr) <= 0) 
	// {
	// 	perror("inet_pton");
	// 	exit(1);
	// }
	dirs = _config_vec.back().get_dirs();
	value = dirs["listen"].back();
	if (dirs.find("listen") == dirs.end())
	{
		std::cerr << "DIRECTIVE NOT FOUND" << std::endl;
		exit(0);
	}
	
	addr = value.substr(0, value.find(':'));
	port = stoi(value.substr(value.find(':') + 1, value.length() - addr.length()));
	std::cout << "addresse => " << addr << std::endl;
	std::cout << "port => " << port << std::endl;
	server_adress.sin_port = htons(port); 
	if (bind(fds.back(),(struct sockaddr *)&server_adress,sizeof(server_adress))) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	}
	if (listen(fds.back(), 5) < 0) 
	{ 
		perror("In listen"); 
		exit(EXIT_FAILURE); 
	}
}



void	Server::acceptConnections(int socketfd, fd_set& current_sockets, std::vector<Client>& clients)
{
	int 	client_fd;
	struct sockaddr_in client_info;

	socklen_t lent_client = sizeof(client_info);

	if ((client_fd =  accept(socketfd, (struct sockaddr *)&client_info, &lent_client)) < 0)
	{
		printf("id = %d", client_fd);
		perror("In accept");            
		exit(EXIT_FAILURE);        
	}
	// int status = fcntl(client_fd, F_SETFL, O_NONBLOCK);

	// if (status == -1)
	// {
	// 	perror("calling fcntl");
	// 	exit(EXIT_FAILURE);        
	// }

	if (client_fd > Server::_max_fd)
		Server::_max_fd = client_fd;
	printf("New client connected from %s:%d (socket: %d)\n", inet_ntoa(client_info.sin_addr), ntohs(client_info.sin_port), client_fd);	
	FD_SET(client_fd, &current_sockets);	
	clients.push_back(Client(client_fd));	
	clients.back()._init_time = get_time();

}

void	Server::readFromClient(int socket, int i)
{
	
	char buffer[buffer_size + 1];
	std::memset(buffer, 0, buffer_size + 1);

	int nbytes = read(socket, buffer, buffer_size);

	if(nbytes == 0)
	{
		closeConnection(clients[i].getSocketFd(), i);
		return ;
	}
	if(nbytes == -1)
	{
		std::cout << "ERROR IN READ" << std::endl;
		exit(1);
	}
	// std::cout << "bytes readed : " << nbytes << std::endl;
	clients[i].request.append(buffer, nbytes);
	int		pos;

	pos = clients[i].request.find("\r\n\r\n");

	if (clients[i].request.find("\r\n\r\n") != std::string::npos)
	{
		clients[i].getRequest().parse_request(clients[i].request);
		clients[i].getRequest().isReqWellFormed(clients[i].getSocketFd());
	}
	if (!clients[i].getRequest().get_request_line().getPath().empty())
	{
    	std::map<std::string, std::string> directives = clients[i].getRequest().get_request_header().get_directives();
		int max_body_size;

		clients[i].getResObj().setConfig(_config_vec);
		max_body_size = clients[i].getResObj().getMacthedServer(clients[i].getRequest().get_request_header().get_directives()["Host"]).getClientMaxBodySize();
		if (directives.find("Content-Length") != directives.end())
		{
			_max_buff = std::stoi(directives["Content-Length"]);

			
			// i have to convert body size to bit before compare values
			std::cout << "MAX BODY SIZE : " << max_body_size << std::endl;

			if (_max_buff > max_body_size)
        		throw "413 Request Entity Too Large";
		}
		if (directives.find("Transfer-Encoding") != directives.end() && directives["Transfer-Encoding"] == "chunked"
				&& clients[i].getRequest().get_request_line().getMethod() == "POST")
		{
			int j = pos + 4;

			std::string hex_body_value = "";

			while (clients[i].request[j])
			{
				if (clients[i].request[j] == '\n')
					break ;
				hex_body_value += clients[i].request[j];
				j++;
			}

			int body_length = clients[i].getRequest().hexToInt(hex_body_value);
			
			if (body_length < buffer_size)
				buffer_size = body_length;
			_body_size += body_length;
			if (_body_size > max_body_size)
        		throw "413 Request Entity Too Large";
			std::cout << "body value " << body_length << std::endl;
		}
	}
	// clients[i].request += buffer;

	//GET HEADERS VALUE 
	if (nbytes < 1024)
	{
		// std::cout << "-------REQUEST--------" << std::endl;
		// std::cout << clients[i].request << std::endl;
		std::string buff = buffer;

		pos = clients[i].request.find("\r\n\r\n") + 4;
		clients[i].request.erase(0, pos);
		std::cout << "BODY : " << clients[i].request << std::endl;
		clients[i].request.clear();
		
		// body 

		// clients[i].getRequest().isReqWellFormed(clients[i].getSocketFd());
	
		// std::cout << "----------------------" << std::endl;
		// exit(0);
		FD_SET(socket, &write_sockets);
	}
	clients[i]._init_time = get_time();

}

void	Server::writeToClient(int sock, int i)
{
	// std::string reply = "HTTP/1.1 200 OK\r\nContent-Length: 17\r\n\r\nHello from server";

	clients[i].setResObj(Response(clients[i].getRequest()));

	clients[i].getResObj().setConfig(_config_vec);
	clients[i].getResObj().handleRequest(clients[i].getSocketFd());
	// send response
	// clients[i].getResObj();
	// std::cout << "|================RESPONSE================|" << std::endl;
    // std::cout << clients[i].getResObj().getResponse();
    // std::cout << "|====================================|" << std::endl;
	write(sock, clients[i].getResObj().getResponse().c_str(), clients[i].getResObj().getResponse().length());
	// clients[i].handle_request_errors();
	FD_CLR(sock, &write_sockets);
	// FD_CLR(sock, &current_sockets);
	// close(sock);
	// clients.erase(clients.begin() + i);
	clients[i]._init_time = get_time();

}

void	Server::closeConnection(int sock, int i)
{
	std::cout << "Closing Connection from " << sock << " ..." << std::endl;
	FD_CLR(sock, &current_sockets);
	FD_CLR(sock, &write_sockets);
	close(sock);
	clients.erase(clients.begin() + i);
}

void	Server::establishConnections()
{
	int 	i, max_fd, status;
	int client_fd;
	struct sockaddr_in client_info;
	struct timeval timeout;
	long	current_time;
	int		sel_ret;

	i = -1;
	//initialize my current set

	FD_ZERO(&current_sockets);
	FD_ZERO(&write_sockets);
	while (++i < fds.size())
		FD_SET(fds[i], &current_sockets);
	while (1)
	{
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;

		// Clear the read set for this iteration
		FD_ZERO(&ready_sockets);

		ready_sockets = current_sockets;
		std::cout << "Before select call" << std::endl;

		// the timeout value specify to select the period to check if my fds is ready or not 
	
		sel_ret = select(Server::_max_fd + 1, &ready_sockets, &write_sockets, NULL, &timeout);
		if (sel_ret < 0)
		{
			perror("select error");
			exit(EXIT_FAILURE);
		}	
		else if (sel_ret == 0)
			std::cout << "Nothing on ports" << std::endl;

		i = -1;
		std::cout << "Waiting for incoming connection..." << std::endl;
		while (++i < fds.size())
		{
			if (FD_ISSET(fds[i], &ready_sockets))
				acceptConnections(fds[i], current_sockets, clients);	
		}
		i = -1;
		std::cout << "client size : " << clients.size() << std::endl;
		while (!clients.empty() && ++i < clients.size())
		{
			std::cout << "ENTER CLIENT LOOP " << std::endl;
			std::cout << "clients[i]._init_time : " << clients[i]._init_time << std::endl;
			if (get_time() - clients[i]._init_time > 5)
			{
				std::cout << "passed time ==> " << get_time() - clients[i]._init_time << std::endl;
				closeConnection(clients[i].getSocketFd(), i);
				continue ;
			}
			if (FD_ISSET(clients[i].getSocketFd(), &ready_sockets))
			{
				try
				{
					readFromClient(clients[i].getSocketFd(), i);
				}
				catch (const char * excep)
				{
					 std::cout << "parsing excep : " << excep << std::endl;
					std::string exp = excep;
					clients[i].getResObj().getResponseLine().setStatus(exp.substr(0, exp.find(" ")));
					clients[i].getResObj().getResponseLine().setMessage(exp.substr(exp.find(" ") + 1, 
							exp.length() - clients[i].getResObj().getResponseLine().getStatus().length()));
    				clients[i].getResObj().createResponse(exp);
					write(clients[i].getSocketFd(), clients[i].getResObj().getResponse().c_str(), clients[i].getResObj().getResponse().length());
					std::cout << clients[i].getResObj().getResponse() << std::endl;
					closeConnection(clients[i].getSocketFd(), i);
					continue ;
				}
			}
			if (FD_ISSET(clients[i].getSocketFd(), &write_sockets))
			{
				writeToClient(clients[i].getSocketFd(), i);
			}
		}
		
	}
}

int		main(int ac, char **av)
{
	
	int		server_fd;

	if (!av[1])
	{
		std::cout << "a config file needed" << std::endl;
		return (1);
	}
	
	// creating a socket

	// get server config , parse it && create sockets and bind them 

	Server server(av[1]);

	// try {
		server.establishConnections();
	// } catch (char const* exp) {
	// 	std::cout << exp << std::endl;
	// }
	return (0);
}

// parse only headers value
// GET => detect carriage return value
// POST => 
