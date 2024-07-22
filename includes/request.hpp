
#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <map>
#include "request_line.hpp"
#include "request_header.hpp"
#include <iostream>
#include <string.h>
#include <sstream>

class Request
{
    public :

        Request();

        const HttpRequestLine&    get_request_line() const;
        const HttpRequestHeader&  get_request_header() const;
        const std::string&        get_body() const;

        void        set_request_line_values(std::string&);
        void        set_request_header_values(std::string);
        void        handle_request_errors(int sock_fd);        
        void        parse_request(std::string content);
        int         notAllowedChar(const std::string& path);
        void        isReqWellFormed(int sock_fd);
        int         isUriTooLong(int uri_size);
        std::string convertChars(const std::string& path);
        int         hexToInt(const std::string& hexStr);
        
    private :
    
        HttpRequestLine     _request_line;
        HttpRequestHeader   _request_header;
        std::string         _body;
};

#endif