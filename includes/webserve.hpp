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
#include "request.hpp"
#include "config_file.hpp"
#include "response.hpp"
#include "client.hpp"

#define BUFFER_SIZE 1024

typedef struct sockaddr_in SC_IN;

class Response;

class Server
{
    public :
        Server(std::string filename);

        int             getSocketFd() const;
        int             getPort() const;
        std::string     getServerAddr() const;
        void            lunchServer();
        void	        acceptConnections(int socketfd, fd_set& current_sockets, std::vector<Client>& clients, int server_index);
        void            bindSocket();
        void	        establishConnections();

        void	        writeToClient(int sock, int i);
        void	        readFromClient(int socket, int i);
        void	        closeConnection(int sock, int i);

        // static std::map<std::string, std::string > getMimeTypes();
        static void		initializeMap(std::string file_name);
        
        static int      _max_fd;

    private :
        Server();

        int                         _socket_fd;
	    int                         _port;
	    fd_set                      current_sockets, ready_sockets, write_sockets;
        std::string                 _addr;    
        std::vector<int >           fds;
        std::vector<Client >        clients; 
        std::vector<ServerConfig>   _config_vec;
        int                         _max_buff;
	    int		                    buffer_size;
        int                         _body_size;
};

