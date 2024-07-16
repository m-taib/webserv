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

typedef struct sockaddr_in SC_IN;

class Client
{
    public:
        Client(int sc);
        int             getSocketFd() const;
        
        std::string     request;
        int             _init_time;        
        Request&          getRequest();
        Response&         getResObj();
        void              setResObj(Response);

    private :
    
        Client();
        int             _socket;
        Request         _request;
        Response        _response;
        // ServerConfig    _server;
};

class Server
{
    public :
        Server(std::string filename);

        int             getSocketFd() const;
        int             getPort() const;
        std::string     getServerAddr() const;
        void            lunchServer();
        void	        acceptConnections(int socketfd, fd_set& current_sockets, std::vector<Client>& clients);
        void            bindSocket();
        void	        establishConnections();

        void	        writeToClient(int sock, int i);
        void	        readFromClient(int socket, int i);
        void	        closeConnection(int sock, int i);
        
        static int      _max_fd;

    private :
        Server();

        int             _socket_fd;
	    int             _port;
	    fd_set          current_sockets, ready_sockets, write_sockets;
        std::string     _addr;    
        std::vector<int >   fds;
        std::vector<Client > clients; 
        std::vector<ServerConfig> _config_vec;
};

