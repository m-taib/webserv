#include "../includes/request.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>

Request::Request()
{
    
}
const HttpRequestLine&    Request::get_request_line() const
{
    return _request_line;
}

const HttpRequestHeader&  Request::get_request_header() const
{
    return _request_header;
}

const std::string&        Request::get_body() const
{
    return _body;
}

void    Request::set_request_line_values(std::string& request_line)
{
    std::stringstream s(request_line);
    std::string token;
    std::string values[3];
    int     i = -1;
    
   
    while (s >> token)
        values[++i] = token;
    
    _request_line.setMethod(values[0]);
    _request_line.setPath(values[1]);
    _request_line.setHttpVersion(values[2]);
    _request_line.setQueryParams("");
    std::cout << "method : " << _request_line.getMethod() << std::endl;
    std::cout << "path : " << _request_line.getPath() << std::endl;
    std::cout << "http_version : |" << _request_line.getHttpVersion() << "|" << std::endl;
}

void    Request::set_request_header_values(std::string request_header)
{
    std::stringstream s(request_header);
    std::string token;
    std::string key;
    std::string value;

    _body = "";
    while (getline(s, token))
    {
        if (token == "\r")
        {
            while (getline(s, token))
                _body += token;
   
            _request_header.set_req_state(1);
            break ;
        }
        token = strtok((char *)token.c_str(),"\r");

        key = token.substr(0, token.find(':'));
        value = token.substr(token.find(':') + 2, (token.length() - key.length() - 2));
        std::cout << key << "=>" << value << std::endl; 

        _request_header.set_directives(key, value);
    }
}

void	Request::parse_request(std::string content)
{
	//set request line
	//set request header 
	//set body 
	std::string token;
	std::stringstream s(content);
	std::cout << "|---------REQUEST LINE---------|" << std::endl;
	getline(s,token);
	// std::cout << "------BEFORE-------" << std::endl;
	// std::cout << content << std::endl;
	set_request_line_values(token);
	content.erase(0 , content.find('\n') + 1);
	// std::cout << "------AFTER--------" << std::endl;
	// std::cout << request << std::endl;
	std::cout << "|------REQUEST HEADER----------|" << std::endl;
	set_request_header_values(content);
	std::cout << "|------------------------------|" << std::endl;
}

int             Request::hexToInt(const std::string& hexStr) 
{
    return std::stoi(hexStr, nullptr, 16);
}

std::string     Request::convertChars(const std::string& path)
{
    std::string str = "";
    std::string allowed_chars;
    std::string nums = "0123456789";
    std::string num = "";

    allowed_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%";
    for (int i = 0; i < path.size() ; i++)
    {
        num.clear();
        if (path[i] != '%' && allowed_chars.find(path[i]) != std::string::npos)
            str += path[i];
        else
        {
            if (path[++i])
            {
                while (path[i] && nums.find(path[i]) != std::string::npos)
                    num += path[i++];

                if (!num.empty())
                {
                    num = hexToInt(num); 
                    str += num;
                }
            }
        }
    }

    return str;
}

int      Request::notAllowedChar(const std::string& path)
{
    std::string allowed_chars;
    std::string converted_str;

    allowed_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%";
    
    converted_str = convertChars(path);
    for(int i = 0; i < converted_str.size(); i++)
    {
        if (allowed_chars.find(converted_str[i]) == std::string::npos)
            return (1);
    }
    return (0);
}

int                Request::isUriTooLong(int uri_size)
{
    if (uri_size > 2048)
        return (1);
    return (0);
}

void    Request::isReqWellFormed(int sock_fd)
{
    std::map<std::string, std::string> directives = get_request_header().get_directives();

    if ((directives.find("Host") == directives.end()  
        || directives["Host"].empty() 
        || !get_request_header().get_req_state()))
        throw "400 Bad Request";
    else if (directives.find("Transfer-Encoding") != directives.end() 
            && directives["Transfer-Encoding"] != "chunked" && directives.find("Content-Length") != directives.end() 
            && directives["Content-Length"].empty())   
    {
        std::cout << "Transfer-Encoding error " << std::endl;
        throw "400 Bad Request";
    }
    else if (directives.find("Transfer-Encoding") != directives.end() && directives["Transfer-Encoding"] != "chunked")
       throw "501 Not Implemented";
    else if ((directives.find("Transfer-Encoding") == directives.end() 
            && directives.find("Content-Length") == directives.end() 
            && _request_line.getMethod() == "POST") || notAllowedChar(_request_line.getPath()))   
       throw "400 Bad Request";
    else if (isUriTooLong(_request_line.getPath().length()))
        throw "414 Request-URI Too Long";
}



