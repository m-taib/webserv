
#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <map>
#include "request_line.hpp"
#include "request_header.hpp"
#include <iostream>
#include <string.h>
#include <sstream>
#include <fstream>
#include <vector>
#include <unistd.h>

class Request
{
    public :

        Request();
        Request(const Request&);
        Request& operator=(const Request&);
        ~Request();
        
        HttpRequestLine&    getRequestLine();
        HttpRequestHeader&  getRequestHeader();
        std::string&        getBody();

        void                clearRequest();

        void        setRequestLineValues(std::string&);
        void        setRequestHeaderValues(std::string);
        void        handleRequestErrors(int sock_fd);        
        void        parseRequest(std::string content);
        int         notAllowedChar(const std::string& path);
        void        isReqWellFormed(int sock_fd, int);
        int         isUriTooLong(int uri_size);
        std::string convertChars(const std::string& path);
        int         hexToInt(const std::string& hexStr);
        void        setBody(std::string&, int);
        void        setEnv(char **env);
        char        **getEnv();
        void        appendInFile(std::string chunk);
        std::fstream&   getFile();

    private :
    
        HttpRequestLine             _request_line;
        HttpRequestHeader           _request_header;
        std::string    _body;
        std::vector<std::string > body_vec;    
        char **_env;
	    std::fstream    file;
        std::fstream    body;
};

#endif