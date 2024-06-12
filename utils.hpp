#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>





// // Client side C/C++ program to demonstrate Socket programming
// #include <stdio.h>
// #include <sys/socket.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <netinet/in.h>
// #include <string.h>
// #include <arpa/inet.h>

// #define PORT 8080

// int main(int argc, char const *argv[])
// {
//     int sock = 0; long valread;
//     struct sockaddr_in serv_addr;
//     char *hello = "Hello from client";
//     char buffer[1024] = {0};
//     if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
//     {
//         printf("\n Socket creation error \n");
//         return -1;
//     }
    
//     memset(&serv_addr, 0, sizeof(serv_addr));
    
//     serv_addr.sin_family = AF_INET;
//     serv_addr.sin_port = htons(PORT);
    
//     // Convert IPv4 and IPv6 addresses from text to binary form
//     if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
//     {
//         printf("\nInvalid address/ Address not supported \n");
//         return -1;
//     }
    
//     if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
//     {
//         printf("\nConnection Failed \n");
//         return -1;
//     }
//     send(sock , hello , strlen(hello) , 0 );
//     printf("Hello message sent\n");
//     valread = read( sock , buffer, 1024);
//     printf("%s\n",buffer );
//     return 0;
// }



// // Server side C program to demonstrate Socket programming
// #include <stdio.h>
// #include <sys/socket.h>
// #include <unistd.h>
// #include <stdlib.h>
// #include <netinet/in.h>
// #include <string.h>

// #define PORT 8080
// int main(int argc, char const *argv[])
// {
//     int server_fd, new_socket; long valread;
//     struct sockaddr_in address;
//     int addrlen = sizeof(address);
    
//     char *hello = "Hello from server";
    
//     // Creating socket file descriptor
//     if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
//     {
//         perror("In socket");
//         exit(EXIT_FAILURE);
//     }
    

//     address.sin_family = AF_INET;
//     address.sin_addr.s_addr = INADDR_ANY;
//     address.sin_port = htons( PORT );
    
//     memset(address.sin_zero, '\0', sizeof address.sin_zero);
    
    
//     if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
//     {
//         perror("In bind");
//         exit(EXIT_FAILURE);
//     }
//     if (listen(server_fd, 10) < 0)
//     {
//         perror("In listen");
//         exit(EXIT_FAILURE);
//     }
//     while(1)
//     {
//         printf("\n+++++++ Waiting for new connection ++++++++\n\n");
//         if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
//         {
//             perror("In accept");
//             exit(EXIT_FAILURE);
//         }
        
//         char buffer[30000] = {0};
//         valread = read( new_socket , buffer, 30000);
//         printf("%s\n",buffer );
//         write(new_socket , hello , strlen(hello));
//         printf("------------------Hello message sent-------------------\n");
//         close(new_socket);
//     }
//     return 0;
// }