#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <iostream>
#include <_types/_intmax_t.h>
#include <stdlib.h>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/select.h>
#include <fcntl.h>
#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <stdlib.h>
#include <sys/time.h>

int		main()
{
	int _socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	
	// int status = fcntl(_socket_fd, F_SETFL,  O_NONBLOCK);

	// if (status == -1)
	// {
	// 	perror("calling fcntl");
	// 	exit(EXIT_FAILURE);        
	// }
	// set socket options 
	int	reuse_addr = 1;

	setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof(reuse_addr));
	
	struct sockaddr_in server_adress;
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
	
	
	addr = "0.0.0.0";
	port = 8080;
	std::cout << "addresse => " << addr << std::endl;
	std::cout << "port => " << port << std::endl;
	server_adress.sin_port = htons(port); 
	if (bind(_socket_fd,(struct sockaddr *)&server_adress,sizeof(server_adress))) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	}
	if (listen(_socket_fd, 5) < 0) 
	{ 
		perror("In listen"); 
		exit(EXIT_FAILURE); 
	}
	char buffer[1024];

	if (accept(_socket_fd,buffer, 1024) == -1)
	{
		perror("In read"); 
		exit(EXIT_FAILURE); 
	}

	// std::cout << "NBYTES : " << nbytes << std::endl;
}
