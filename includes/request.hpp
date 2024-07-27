
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

class Request
{
    public :

        Request();

        const HttpRequestLine&    getRequestLine() const;
        const HttpRequestHeader&  getRequestHeader() const;
        const std::string&        getBody() const;

        void        setRequestLineValues(std::string&);
        void        setRequestHeaderValues(std::string);
        void        handleRequestErrors(int sock_fd);        
        void        parseRequest(std::string content);
        int         notAllowedChar(const std::string& path);
        void        isReqWellFormed(int sock_fd, int);
        int         isUriTooLong(int uri_size);
        std::string convertChars(const std::string& path);
        int         hexToInt(const std::string& hexStr);
        void        setBody(std::string, int);

    private :
    
        HttpRequestLine             _request_line;
        HttpRequestHeader           _request_header;
        std::string    _body;
        std::vector<std::string > body_vec;    
    
};

#endif