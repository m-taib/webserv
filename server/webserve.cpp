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
Server::Server(std::string filename, char **env)
{
	std::fstream    file;
    std::string     line;
	std::string 	token;
	std::string		host;

	_env = env;
    file.open(filename.c_str(), std::ios::in);
    if (file.is_open())
    {
        while (getline(file, line))
        {
			if (line.empty())
				continue ;
			if (line == "server")
			{
				_config_vec.push_back(ServerConfig(file));
				_config_vec.back().extract_data();

				std::vector<std::string > ports = _config_vec.back().get_dirs()["listen"];

				for (int i = 0; i < ports.size(); i++)
				{
					std::cout << ports[i] << " ";
				}
				std::cout << std::endl;

				// exit(0);
				std::cout << "CONFIG VEC SIZE " << _config_vec.size() << std::endl;
				if (_config_vec.back().get_dirs()["listen"].size() > 1)
				{
					std::vector<std::string > ports = _config_vec.back().get_dirs()["listen"];

					_config_vec.back().get_dirs()["listen"].clear();
					_config_vec.back().get_dirs()["listen"].push_back(ports[0]);

					for (int i = 1 ; i < ports.size() ; i++)
					{	
						_config_vec.push_back(_config_vec.back());

						_config_vec.back().get_dirs()["listen"].clear();
						_config_vec.back().get_dirs()["listen"].push_back(ports[i]);

					}
				}
			}

			host  = _config_vec.back().get_dirs()["listen"].front();
			int 	i = 0;

			std::cout << "SEARCHED HOST VALUE : " << _config_vec.back().get_dirs()["listen"].front() << std::endl;

			for (i = 0; i < _config_vec.size() - 1; i++)
			{
				if (_config_vec[i].get_dirs()["listen"].front() == host)
				{
					std::cout << "detect redundent value " << std::endl;
					break ;
				}
			}
			if (i == _config_vec.size() - 1)
			{
				lunchServer();
				bindSocket();
			}
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



void	Server::acceptConnections(int socketfd, fd_set& current_sockets, std::vector<Client>& clients, int server_index)
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
	clients.back()._addr = _config_vec[server_index].get_dirs()["listen"].front();
}

// read header directive
// check if there is a redirection 
// if content length found , read with it and set body
// if transfer-encoding found and chunked , read with chunk size , erase readed part till you find size = 0

void	Server::readFromClient(int socket, int i)
{
	
	char buffer[buffer_size + 1];
	std::memset(buffer, 0, buffer_size + 1);

	int nbytes = read(socket, buffer, buffer_size);
	int max_body_size = 0;


	if (nbytes == 0)
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
	
	if (clients[i].request.find("\r\n\r\n") != std::string::npos)
	{
		
		clients[i].getRequest().parseRequest(clients[i].request);
		clients[i].getRequest().setEnv(_env);
		clients[i].setResObj(Response(clients[i].getRequest()));

		std::map<std::string, std::string> dirs = clients[i].getRequest().getRequestHeader().get_directives();
		clients[i].getResObj().setMacthedServer(_config_vec, clients[i]._addr, dirs["Host"]);

		max_body_size = clients[i].getResObj().getConfig().getClientMaxBodySize();
		clients[i].getRequest().isReqWellFormed(clients[i].getSocketFd(), max_body_size);

		std::cout << "REQUEST PATH " << clients[i].getRequest().getRequestLine().getPath() << std::endl;
       	clients[i].getResObj().setLocations(clients[i].getResObj().getConfig().get_locations());
		
		clients[i].getResObj().setMacthedLocation(clients[i].getRequest().getRequestLine().getPath());
    	
		clients[i].getResObj().set_location_vars();
	
        if (clients[i].getResObj().isLocationHaveRedirection())
		{
			clients[i].request.clear();
			throw "301 Moved Permanently";
		}

		int		pos;
		
		pos = clients[i].request.find("\r\n\r\n");
		
		clients[i].request.erase(0, pos + 4);

	}
	// clients[i].request += buffer;

	//GET HEADERS VALUE 
	if (nbytes < buffer_size)
	{
		// std::cout << "-------REQUEST--------" << std::endl;
		// std::cout << clients[i].request << std::endl;
		if (clients[i].getRequest().getRequestLine().getMethod() == "POST")
		{
			std::cout << "SET BODY " << std::endl;
			clients[i].getRequest().setBody(clients[i].request, max_body_size);
			// std::cout << "|=================BODY VAL=================|" << std::endl;
			// std::cout << clients[i].getRequest().getBody() << std::endl;
			// std::cout << "|==========================================|" << std::endl;
		}
		
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

	std::cout << "START RESPONDING TO CLIENT " << std::endl;
	clients[i].getResObj().handleRequest(clients[i].getSocketFd());
	// send response
	// clients[i].getResObj();
	std::cout << "|================RESPONSE================|" << std::endl;
    std::cout << clients[i].getResObj().getResponse() << std::endl;
    std::cout << "|====================================|" << std::endl;
	write(sock, clients[i].getResObj().getResponse().c_str(), clients[i].getResObj().getResponse().length());
	// clients[i].handle_request_errors();
	FD_CLR(sock, &write_sockets);
	// std::cout << "CLEAR CLIENT SOCK NUM => " << sock << std::endl;
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
				acceptConnections(fds[i], current_sockets, clients, i);	
		}
		i = -1;
		while (++i < clients.size())
		{
			std::cout << "clients[i]._init_time : " << clients[i]._init_time << std::endl;
			if (get_time() - clients[i]._init_time > 5)
			{
				std::cout << "passed time ==> " << get_time() - clients[i]._init_time << std::endl;
				closeConnection(clients[i].getSocketFd(), i);
				continue ;
			}
			if (FD_ISSET(clients[i].getSocketFd(), &ready_sockets))
			{
				std::cout << "A CLIENT IS READY TO READ " << std::endl;
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
					write(clients[i].getSocketFd(), clients[i].getResObj().getResponse().c_str(), 
							clients[i].getResObj().getResponse().length());
					// std::cout << clients[i].getResObj().getResponse() << std::endl;
					if (clients[i].getResObj().getResponseLine().getStatus() != "301")
					{
						std::cout << "CLOSED CONNECTION AFTER RESPONSE" << std::endl;
						closeConnection(clients[i].getSocketFd(), i);
					}
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

int		main(int ac, char **av, char **env)
{
	
	int		server_fd;

	if (!av[1])
	{
		std::cout << "a config file needed" << std::endl;
		return (1);
	}
	
	// creating a socket

	// get server config , parse it && create sockets and bind them 

	// std::cout << "ENVIRONMENT VARS" << std::endl;
	// int i;
	// for (i = 0; env[i]; i++)
	// {
	// 	std::cout << env[i] << std::endl;
	// }
	// std::cout << "size : " << i << std::endl;
	// exit(0);
	Server server(av[1], env);

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
