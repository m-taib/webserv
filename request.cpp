#include "request.hpp"
#include <iostream>
#include <sstream>
#include <string>

Request::Request()
{

}
// const HttpRequestLine&    Request::get_request_line() const
// {
//     return _request_line;
// }

// const HttpRequestHeader&  Request::get_request_header() const
// {
//     return _request_header;
// }

const std::string&        Request::get_body() const
{
    return _body;
}

void    Request::set_request_line_values(std::string& request_line)
{
    std::string values[3];
    int     i = -1;
    char * token;

    token = strtok((char *)request_line.c_str()," ");
    while (token != NULL)
    {
        values[++i] = token;
        token = strtok(NULL, " ");
    }

    _request_line.set_method(values[0]);
    _request_line.set_path(values[1]);
    _request_line.set_http_version(values[2]);
    _request_line.set_query_params("");
    std::cout << "method : " << _request_line.get_method() << std::endl;
    std::cout << "path : " << _request_line.get_path() << std::endl;
    std::cout << "http_version : " << _request_line.get_http_version() << std::endl;
}

void    Request::set_request_header_values(std::string request_header)
{
    std::stringstream s(request_header);
    std::string token;
    std::string key;
    std::string value;

    while (getline(s, token))
    {
        if (token == "\r")
        {
            _body = token.substr(token.find('\r') + 1, token.length());
            std::cout << "------BODY------\n" << _body << std::endl;
            break ;
        }
        key = token.substr(0, token.find(':'));
        value = token.substr(token.find(':') + (token[(token.find(':') + 1)] == ' ') + 1, token.length());
        std::cout << key << "=>" << value << std::endl; 
        _request_header.set_directives(key, value);
    }

}