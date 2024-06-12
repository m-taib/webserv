#include "webserve.hpp"
#include <_types/_intmax_t.h>
#include <codecvt>
#include <cstring>
#include <iterator>
#include <string>
#include <sys/_types/_fd_def.h>
#include <sys/_types/_timeval.h>
#include <vector>

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
			if (line[0] != ' ' && line[0] != '\t')
				continue;
			token = line.substr(line.find('l'), line.length());
			token = token.substr(token.find(' '), token.length());
			std::string addr = token.substr(1, token.find(':') -1);
			std::string port = token.substr(token.find(':') + 1, (token.length() - token.find(':')) - 2);
			// std::cout << addr << ":" << port << std::endl;
			lunchServer();
			bindSocket(addr, stoi(port));
		}
	}
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

Client::Client(int sc)
{
	_socket = sc;
	_init_time = -1;
}

int      Client::getSocketFd() const
{
	return _socket;
}

void	Client::handle_request()
{
	
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
	if (_socket_fd > _max_fd)
		_max_fd = _socket_fd;
	fds.push_back(_socket_fd);
}

void	Server::bindSocket(std::string addr, int port)
{
	struct sockaddr_in server_adress;

	memset((char *)&server_adress, 0, sizeof(server_adress)); 
	server_adress.sin_family = AF_INET; 
	server_adress.sin_addr.s_addr = INADDR_ANY;
	// if (inet_pton(AF_INET, addr.c_str(), &server_adress.sin_addr.s_addr) <= 0) 
	// {
	// 	perror("inet_pton");
	// 	exit(1);
	// }
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
	int buffer_size = 1024;
	char buffer[buffer_size];
	std::memset(buffer, 0, buffer_size);

	int nbytes = read(socket, buffer, buffer_size);

	if(nbytes == 0 || nbytes == -1)
	{
		std::cout << "ERROR in read" << std::endl;
		exit(1);
	}
	std::cout << buffer << std::endl;
	clients[i].request.append(buffer, nbytes);
	if (nbytes < buffer_size)
	{
		clients[i].handle_request();
		FD_SET(socket, &write_sockets);
	}
	clients[i]._init_time = get_time();

}

void	Server::writeToClient(int sock, int i)
{
	std::string reply = "HTTP/1.1 200 OK\r\nContent-Length: 17\r\n\r\nHello from server";

	write(sock, reply.c_str(), reply.length());
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
				readFromClient(clients[i].getSocketFd(), i);
			if (FD_ISSET(clients[i].getSocketFd(), &write_sockets))
				writeToClient(clients[i].getSocketFd(), i);
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

	// 

	server.establishConnections();
	return (0);
}
