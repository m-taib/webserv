#include <arpa/inet.h>
#include <cstdio>
#include <unistd.h>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <iostream>

int main()
{
    // std::cout << "helllloooooo\rWorrld" << std::endl;
    int sock;
    struct sockaddr_in  server_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1){
        perror("Error creating the socket");
        return (0);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(8181);

    std::cout << "waiting to connect to server..." << std::endl;
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1){
        perror("Error connecting to the server");
        close(sock);
        return (0);
    }

    write(sock, "*-------*hello from the client*-------*\n", 41);

    char    buffer[1024];
    int valread = read(sock, buffer, 1024);
    buffer[valread] = '\0';

    std::cout << "Msg from the server: " << buffer << std::endl;

}