
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

        // const HttpRequestLine&    get_request_line() const;
        // const HttpRequestHeader&  get_request_header() const;
        const std::string&        get_body() const;

        void    set_request_line_values(std::string&);
        void    set_request_header_values(std::string);

    private :
    
        HttpRequestLine     _request_line;
        HttpRequestHeader   _request_header;
        std::string         _body;
};
#endif