#include "utils.hpp"
#include <arpa/inet.h>
#include <iostream>
#include <fcntl.h>

int		main()
{
	//create a socket

	int		net_sock;
	struct sockaddr_in server_address;
	int		con_status;

	net_sock = socket(AF_INET, SOCK_STREAM, 0);
	int status = fcntl(net_sock, F_SETFL,  O_NONBLOCK);
	if (status == -1)
	{
		perror("calling fcntl");
		exit(EXIT_FAILURE);        
	}
	if (net_sock == -1)
		printf("Error in creating socket\n");

	// specify an adress for the socket
	
	printf("fd client : %d\n" , net_sock);
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(8080);
	// server_address.sin_addr.s_addr = INADDR_ANY;


	 if(inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
	std::cout << "connecting..." << std::endl;
	con_status = connect(net_sock, (struct sockaddr *) &server_address, sizeof(server_address));
	std::cout << "Connected.." << std::endl;

	//check error for the connection;
	
	if (con_status == -1)
		printf("There was an error making a connection\n");
	
	//recieve data from the server
	
	char client_request[] = "Hello from client, its me MARIO";

	write(net_sock, client_request, sizeof(client_request));
	char buffer[1024]= {0};
	// read(net_sock, buffer, 102);
	// printf("%s\n",buffer);
	// then close the socket
	
	close(net_sock);
	return (0);
}	
