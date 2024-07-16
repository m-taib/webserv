#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/select.h>

#define MAX_CLIENTS 10  // Maximum number of clients to handle concurrently

int main() {
  int server_socket, client_socket;
  struct sockaddr_in server_address, client_address;
  int addrlen = sizeof(server_address);
  int max_socket_fd;
  fd_set read_fds;

  char buffer[1024];

  // Create a socket
  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == -1) {
    perror("socket");
    exit(1);
  }

  // Set socket options for reusability
  int reuse_addr = 1;
  if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof(reuse_addr)) == -1) {
    perror("setsockopt");
    exit(1);
  }

  // Configure server address
  memset(&server_address, 0, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = INADDR_ANY; // Listen on any available address
  server_address.sin_port = htons(8080);  // Listen on port 8080

  // Bind the socket to the address
  if (bind(server_socket, (struct sockaddr*)&server_address, addrlen) == -1) {
    perror("bind");
    exit(1);
  }

  // Listen for incoming connections
  if (listen(server_socket, MAX_CLIENTS) == -1) {
    perror("listen");
    exit(1);
  }

  printf("Server listening on port 8080...\n");

  // Initialize the file descriptor set
  FD_ZERO(&read_fds);

  // Main loop for handling connections
  while (1) {
    // Set the maximum file descriptor value
    max_socket_fd = server_socket;

    // Clear the read set for this iteration
    FD_ZERO(&read_fds);

    // Add server socket to the read set for monitoring new connections
    FD_SET(server_socket, &read_fds);

    // Add connected clients to the read set for monitoring data
   

    // Wait for events on sockets with a timeout of 5 seconds
    struct timeval timeout = {5, 0};
    int ret = select(max_socket_fd + 1, &read_fds, NULL, NULL, &timeout);

    if (ret == -1) {
      perror("select");
      exit(1);
    } else if (ret == 0) {
      printf("Timeout! No events for 5 seconds.\n");
    } else {
      // Handle events on the sockets
      // Iterate through the file descriptors in the read set
      for (int i = 0; i <= max_socket_fd; i++) {
        if (FD_ISSET(i, &read_fds)) {
          if (i == server_socket) {
            // New connection!
            addrlen = sizeof(client_address);
            client_socket = accept(server_socket, (struct sockaddr*)&client_address, (socklen_t *)&addrlen);
            if (client_socket == -1) {
              perror("accept");
              exit(1);
            }
            printf("New client connected from %s:%d (socket: %d)\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port), client_socket);
          }}}}
          }
}
