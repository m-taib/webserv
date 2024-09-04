#include "request.hpp"
#include "config_file.hpp"
#include "response.hpp"
#include <string>

class Response;

class Client
{
    public:
        Client(int sc);
        
        std::string         request;
        std::string         tmp;
        int                 _init_time;      
        
        int                 getSocketFd() const;
        Request&            getRequest();
        Response&           getResObj();
        void                setResObj(Response);
        void                setIndex(std::string& index);
        std::string     _addr;

    private :
    
        Client();
        int             _socket;
        Request         _request;
        Response        _response;
        // ServerConfig    _server;
};