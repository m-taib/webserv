#include "../includes/response.hpp"
#include <_stdio.h>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/_types/_size_t.h>
#include <unistd.h>
#include <vector>

Response::Response() 
{
   
}

Response::Response(Request request) 
{ 

    // _request = request;
    _http_versions.push_back("HTTP/0.9");
    _http_versions.push_back("HTTP/1.0");
    _http_versions.push_back("HTTP/2");
    _http_versions.push_back("HTTP/3:");

    _http_methods.push_back("GET");
    _http_methods.push_back("POST");
    _http_methods.push_back("DELETE");
    _http_methods.push_back("PUT");
    _http_methods.push_back("HEAD");
    _http_methods.push_back("OPTIONS");
    _http_methods.push_back("TRACE");
    _http_methods.push_back("CONNECT");
    _http_methods.push_back("PATCH");

    // _directives = getMacthedLocation(_request.get_request_header().get_directives()["PATH"]);


}
const HttpResponseLine&    Response::getResponseLine() const
{
    return _response_line;
}

const HttpResponseHeader&  Response::getResponseHeader() const
{
    return _response_header;
}

const std::string&        Response::getBody() const
{
    return _body;
}

const std::string&        Response::getResponse() const
{
    return _body;
}

ServerConfig&     Response::getMacthedServer(std::string& host)
{
    for (size_t i = 0; i < _config_vec.size() ; i++)
    {
        // you have to compare it to all host value

        if (host == _config_vec[i].get_dirs()["listen"].front())
        {
            return _config_vec[i];
        }
    }
    throw "HOST VALUE NOT MATCHED";
}

void     Response::getMacthedLocation(std::string path)
{
     if (path.rfind("/") == std::string::npos)
        return ;
    for (size_t i = 0; i < _locations.size() ; i++)
    {
        if (path == (_locations[i].first.c_str() + 1))
            location_dirs = _locations[i].second;
    }
    getMacthedLocation(path.substr(0, path.rfind("/")));
}

void        Response::setConfig(std::vector<ServerConfig>& config_vec)
{
    _config_vec = config_vec;
}

void    Response::set_location_vars()
{
    if (location_dirs.find("root") != location_dirs.end())
		_conf.setRoot(location_dirs["root"].front());
	if (location_dirs.find("redirection") != location_dirs.end())
		_conf.setRedirection(location_dirs["redirection"].front());
	if (location_dirs.find("autoindex") != location_dirs.end())
		_conf.setAutoIndex(location_dirs["autoindex"].front());
	if (location_dirs.find("client_max_body_size") != location_dirs.end())
		_conf.setClientMaxBodySize(stol(location_dirs["client_max_body_size"].front()));
	if (location_dirs.find("error_log") != location_dirs.end())
		_conf.setErrorLog(location_dirs["error_log"].front());
	if (location_dirs.find("access_log") != location_dirs.end())
		_conf.setAccessLog(location_dirs["access_log"].front());
}

void                Response::checkHttpVersion(const std::string& http_version)
{
    if (std::find(_http_versions.begin(), _http_versions.end(), http_version) != _http_versions.end()
            && http_version != "HTTP/1.1")
        throw "HTTP/1.1 505 HTTP Version Not Supported";
    else if (std::find(_http_versions.begin(), _http_versions.end(), http_version) == _http_versions.end())
        throw "HTTP/1.1 400 Bad Request";
}

void                Response::checkMethodValidity(const std::string& method, std::vector<std::string>& methods)
{
    int     i;

    if (std::find(_http_methods.begin(), _http_methods.end(), method) == _http_methods.end())
        throw "HTTP/1.1 400 Bad Request";
    else
    {
        for (i = 0; i < methods.size(); i++)
        {
            if (method == methods[i])
                break ;
        }
        if (i != methods.size())
            throw "HTTP/1.1 405 Method Not Allowed";
    }
}


void        Response::checkBodySize(const unsigned long& body_size, int client_max_body_size)
{
    if (body_size > client_max_body_size)
        throw "413 Request Entity Too Large";
}

void       Response::is_location_have_redirection()
{
    if (location_dirs.find("return") != location_dirs.end())
        throw "301 Moved Permanently";
    
}

// int     get_resource_type(std::string& path)
// {

// }

//|=====GET METHOD======|

int     Response::checkPathInRoot(std::string& path)
{
    if (access(path.c_str(), F_OK) == -1)
        return (0);
    return (1);
}

int     Response::getResourceType(std::string& path)
{
    struct stat statbuf;
    int ret;
    
    ret = stat(path.c_str(), &statbuf);

    if (ret == 0) 
    {
        if (S_ISREG(statbuf.st_mode)) 
            return FILE;
        else if (S_ISDIR(statbuf.st_mode)) 
            return DIR;
    }
    else 
        return -1;
}

int     Response::isDirHasIndexFiles()
{
    if (location_dirs.find("index") != location_dirs.end())
        return (1);
    return (0);
}
int     Response::isUriHasSlashInend(const std::string& path)
{
    if (path[path.length() - 1] != '/')
    {
        //location = path += "/";
        return (0);
    }
    return (1);
}


int     Response::getAutoIndex()
{
    if (_conf.get_dirs().find("autoindex") != _conf.get_dirs().end())
    {
        return (1);
    }
    return (OFF);
}

//|=======================|

void       Response::handleGetMethod()
{
    std::string path;

    path = _conf.getRoot();
    path += _request.get_request_line().getPath();

    if (!checkPathInRoot(path))
        throw "HTTP/1.1 404 Not Found";
    if (getResourceType(path) == DIR)
    {
        if (!isUriHasSlashInend(_request.get_request_line().getPath()))
            throw "301 Moved Permanently";
        else if (!isDirHasIndexFiles())
        {
            if (getAutoIndex() == OFF)
                throw "HTTP/1.1 403 Forbidden";
            else 
                throw "HTTP/1.1 200 OK";    
        }
        else 
        {
        
        }
        
    }
}


void    Response::handleRequest(int fd)
{
    std::string path_value;
    std::stringstream ss;
    std::string path;
    std::string response;
    std::string method;

    _conf = getMacthedServer( _request.get_request_header().get_directives()["Host"]);

    try 
    {
        _request.isReqWellFormed(fd);
        checkBodySize(_body.length(), _conf.getClientMaxBodySize());
        _locations = _conf.get_locations();
        path_value = _request.get_request_line().getPath();
        while (std::getline(ss, path_value, '/'));
        std::cout << "PATH VALUE : " << path_value << std::endl;
        checkHttpVersion(_request.get_request_line().getHttpVersion());
        getMacthedLocation(path_value);
        if (location_dirs.empty())
        {
            set_location_vars();
            path_value = (_conf.get_dirs()["root"].front() += path_value);
            //check if path value founded
            //else
            throw "HTTP/1.1 404 Not Found";
            //404 Not Found
        }   
        is_location_have_redirection();
        //check if allowed_method dir inherit its value
        method = _request.get_request_line().getMethod();
        checkMethodValidity(method, location_dirs["allowed_methods"]);
        if (method == "GET")
            handleGetMethod();
    }
    catch (std::string& exp) 
    {
        
    }

    
    // std::cout << "root : " << conf.getRoot() << std::endl;





    exit(0);
    
    // if (!checkMethodValidity(_conf[""]))
    // {

    // }

}










