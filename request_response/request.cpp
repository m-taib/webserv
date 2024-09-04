#include "../includes/request.hpp"
#include <cstdio>
#include <cstring>
#include <ios>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>

Request::Request()
{
    
}

Request::Request(const Request& rhs)
{
    *this = rhs;
}
Request& Request::operator=(const Request& rhs)
{
    _request_line = rhs._request_line;
    _request_header = rhs._request_header;
    _body = rhs._body; 
    body_vec = rhs.body_vec;    
    _env = rhs._env;
    
    return (*this);
}

Request::~Request()
{

}
char**       Request::getEnv()
{
    return _env;
}

void         Request::clearRequest()
{
	_request_line.getMethod().clear();
	_request_line.getHttpVersion().clear();
	_request_line.getPath().clear();
	_request_line.getQueryParams().clear();

    _request_header.get_directives().clear();
    
}
HttpRequestLine&    Request::getRequestLine() 
{
    return _request_line;
}

HttpRequestHeader&  Request::getRequestHeader() 
{
    return _request_header;
}

std::string&        Request::getBody() 
{
    return _body;
}

void        Request::setEnv(char **env)
{
    _env = env;
}

void    Request::setRequestLineValues(std::string& request_line)
{
    std::stringstream s(request_line);
    std::string token;
    std::string values[3];
    std::string query_params = "";
    int     i = -1;
    
   
    while (s >> token)
        values[++i] = token;
    
    _request_line.setMethod(values[0]);
    _request_line.setPath(values[1]);
    _request_line.setHttpVersion(values[2]);
    if (_request_line.getPath().find('?') != std::string::npos)
    {
        query_params = _request_line.getPath().substr(_request_line.getPath().find('?') + 1
                                        , _request_line.getPath().length() - _request_line.getPath().find('?'));
    }
    _request_line.setQueryParams(query_params);
    std::cout << "method : " << _request_line.getMethod() << std::endl;
    std::cout << "path : " << _request_line.getPath() << std::endl;
    std::cout << "http_version : |" << _request_line.getHttpVersion() << "|" << std::endl;
}

void    Request::setRequestHeaderValues(std::string request_header)
{
    std::stringstream s(request_header);
    std::string token;
    std::string key;
    std::string value;

    _body = "";
    while (getline(s, token))
    {
        if (token.rfind("\r") == std::string::npos)    
            throw "400 Bad Request";

        if (token == "\r")
        {
            // while (getline(s, token))
            //     _body += token;
            break ;
        }
        token = strtok((char *)token.c_str(),"\r");

        key = token.substr(0, token.find(':'));
        value = token.substr(token.find(':') + 2, (token.length() - key.length() - 2));
        std::cout << key << "=>" << value << std::endl; 

        _request_header.set_directives(key, value);
    }
}

void	Request::parseRequest(std::string content)
{
	//set request line
	//set request header 
	//set body 
	std::string token;
	std::stringstream s(content);
	std::cout << "|---------REQUEST LINE---------|" << std::endl;
	getline(s,token);
    if (token.rfind("\r") == std::string::npos)    
        throw "400 Bad Request";
    
	// std::cout << "------BEFORE-------" << std::endl;
	// std::cout << content << std::endl;
	setRequestLineValues(token);
	content.erase(0 , content.find('\n') + 1);
	// std::cout << "------AFTER--------" << std::endl;
	// std::cout << request << std::endl;
	std::cout << "|------REQUEST HEADER----------|" << std::endl;
	setRequestHeaderValues(content);

    // if body exist and method is post 

	std::cout << "|------------------------------|" << std::endl;
}

int             Request::hexToInt(const std::string& hexStr) 
{
    // std::cout << "hex value : " << hexStr << std::endl;
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
        {
            std::cout << "FOUND A NOT ALLOWED CHARACHTER" << std::endl;
            return (1);
        }
    }
    return (0);
}

int                Request::isUriTooLong(int uri_size)
{
    if (uri_size > 2048)
        return (1);
    return (0);
}

void    Request::isReqWellFormed(int sock_fd, int max_body_size)
{
    std::map<std::string, std::string> directives = getRequestHeader().get_directives();

    int     content_length = 0;

    if (directives.find("Content-Length") != directives.end())
	{
        content_length = std::stoi(_request_header.get_directives()["Content-Length"]);
        std::cout << "Content Length : " << content_length << " " << "max body size : " << max_body_size << std::endl;
        if (content_length > max_body_size)
		    throw "413 Request Entity Too Large";
    }
    else if (_request_line.getMethod() == "GET" && !_body.empty())
        throw "400 Bad Request";
    else if ((directives.find("Host") == directives.end()  
        || directives["Host"].empty()))
    {
        std::cout << "Host error" << std::endl;
        throw "400 Bad Request";
    }
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
    // std::cout << "REACHED BOTTOM" << std::endl;
}


// void    Request::setBody(std::string& body_val, int max_body_size)
// {
//     std::map<std::string, std::string > dirs = _request_header.get_directives();
//     std::string     token;
//     int             chunk_size;

//     if (dirs.find("Content-Length") != dirs.end())
//         return ;
//     file.open("test.txt", std::ios::in);
//     body.open("body.txt", std::ios::app);
//     if (file.is_open() && body.is_open())
//     {
//         while (1)
//         { 
//             getline(file, token);
//             token = token.substr(0, '\r');
//             chunk_size = hexToInt(token);
//             if (!chunk_size)
//                 break ;
//             char buff[chunk_size + 1];

//             memset(buff, 0, chunk_size + 1);
//             file.seekg(token.size() + 2, std::ios::beg);
//             file.read(buff, chunk_size);
//             file.seekg(chunk_size, std::ios::beg);
//             body.write(buff, chunk_size);
//         }
//         body.close();
//         file.close();
//     }
   
// }

void    Request::setBody(std::string& body_val, int max_body_size)
{
    std::string     token;
    std::string     chunk;
    int             body_size = 0;
    int             chunk_size = 0;
    std::string     body;
    int             i;

    std::string bodyval;
    bodyval = body_val;

    // std::cout << "|=================BODY VAL=================|" << std::endl;
    // std::stringstream s(bodyval);
    
    // while (getline(s, token))
    // {
    //     std::cout << token << std::endl;
    // }
    // std::cout << "|==========================================|" << std::endl;
    std::map<std::string, std::string > dirs = _request_header.get_directives();
    
    std::map<std::string, std::string >::iterator it;
    // for (it = dirs.begin(); it != dirs.end(); it++)
    // {
    //     std::cout << it->first << " => " << it->second << std::endl;
    // }
    if (dirs.find("Content-Length") != dirs.end())
    {
        std::cout << _request_header.get_directives()["Content-Length"] << std::endl;
        std::cout << "FOUNDED" << std::endl;
        _body = body_val;
        return ;
    }
    while (1)
    { 
        chunk.clear();
        token = body_val.substr(0, body_val.find('\r'));
        
        if (token == "0" || body_val.empty())
            break;
        if (!token.empty())
            chunk_size = hexToInt(token);
        body_val.erase(0, token.size() + 2);

        body_size += chunk_size;
        if (body_size > max_body_size)
			throw "413 Request Entity Too Large";

        i = 0;
        chunk = body_val.substr(0, chunk_size);
       
        if (chunk.size() != chunk_size)
            std::cout <<  "400 Bad Request" << std::endl;

        _body += chunk;
        body_val.erase(0, chunk_size);
        body_vec.push_back(chunk);
    }
    std::cout << "|==========BODY==========" << std::endl; 
    std::cout << _body << std::endl;
    std::cout << "|========================|" << std::endl;
    // i = 0;
    // std::cout << "body : " << std::endl;
    // std::cout << _body << std::endl;
    // std::cout << "=======" << std::endl;
    // std::cout << body << std::endl;
   
}

void        Request::appendInFile(std::string chunk)
{
    file.open("test.txt", std::ios::app);
    if (file.is_open())
    {
        file.write(chunk.c_str(), chunk.size());
    }
    file.close();
}

std::fstream&   Request::getFile()
{
    return file;
}



// int    Request::setBody(std::string& body_val, int max_body_size)
// {
//     std::string     token;
//     int             chunk_size = 0;
// 	std::fstream    file;
//     static int      content_length;
//     int             len = 0;

//     file.open("test.txt" , std::ios::app);
//     if (file.is_open())
//     {

//         if (!content_length)
//         {
//             token =  body_val.substr(0, body_val.find('\r'));
//             chunk_size = hexToInt(token);
//             body_val.erase(0,  token.length() + 2);
//             content_length = chunk_size;
//         }
//         if (content_length > body_val.length())
//             len = body_val.length();
//         else 
//             len = content_length;
//         token = body_val.substr(0, len);
//         body_val.erase(0, len);

//         if (body_val.find("\r\n0") != std::string::npos)
//         {
           
//             body_val = body_val.substr(0, body_val.find("\r\n0"));
//             token.append(body_val);
//             file << token;
//             return (0);
//         }

//         content_length -= len;
//         file << token;
//     }
//     file.close();
//     return (len);
// }



