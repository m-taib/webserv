#include "request.hpp"
#include "config_file.hpp"
#include "response.hpp"

class Response;

class Client
{
    public:
        Client(int sc);
        
        std::string         request;
        int                 _init_time;      
        
        int                 getSocketFd() const;
        Request&            getRequest();
        Response&           getResObj();
        void                setResObj(Response);
        void                setIndex(std::string& index);

    private :
    
        Client();
        int             _socket;
        Request         _request;
        Response        _response;
        // ServerConfig    _server;
};