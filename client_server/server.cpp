#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/select.h>
#include <iostream>

int		main(int ac, char**av)
{

	//create the server socket 
	
	int		net_sock;
	struct sockaddr_in server_address;
	int		con_status;

	net_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (net_sock == -1)
		printf("Error in creating socket\n");

	// specify an adress for the socket
	printf("fd server : %d\n" , net_sock);

	int	reuse_addr = 1;

	if (setsockopt(net_sock, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof(reuse_addr)) == -1) 
	{
		perror("setsockopt");
		exit(1);
  	}
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(8080);
	server_address.sin_addr.s_addr = INADDR_ANY;

	// bind the socket to our specified ip and port
	
	bind(net_sock, (struct sockaddr*) &server_address, sizeof(server_address));	
	sleep(5);
	std::cout << "Start listening..." << std::endl;

	listen(net_sock, 5);
	// int		client_socket;
	while(true)
	{
		// if ((client_socket = accept(net_sock, NULL, NULL)) > 0)
		// {
		// 	printf("accepted socket = %d\n", client_socket);
		// 	char server_request[] = "Hello from server, its me MARIO";
		// 	write(client_socket, server_request, sizeof(server_request));
		// }
	
		//send the message
		// send(client_socket, server_message, sizeof(server_message), 0);
		
		char 	buffer[4096];
		memset(buffer, 0, 4096);
		// if (read(client_socket, buffer, 5))
		// {
		// 	if (!buffer[0])
		// 		std::cout << "empty buffer" << std::endl;
		// 	else
		// 	{
		// 		printf("%s\n",buffer);
		// 		exit(0);
		// 	}
		// }
		
		//close the socket 
		// close(client_socket);
	}
	return (0);
}
