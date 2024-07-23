#include "../includes/response.hpp"
#include <_stdio.h>
#include <algorithm>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/_types/_size_t.h>
#include <unistd.h>
#include <variant>
#include <vector>

std::map<std::string, std::string > Response::_mime_types;

Response::Response() 
{
    _response_line.setHttpVersion("HTTP/1.1");
    _response_line.setStatus("200");
    _response_line.setMessage("OK");

}

Response::Response(Request request) 
{ 

    _request = request;
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
    _response_line.setHttpVersion("HTTP/1.1");
    _response_line.setStatus("200");
    _response_line.setMessage("OK");

    _body = "";
}

// alias 
// \r
// not implemenyed method

std::map<std::string, std::string > Response::getMimeTypes() 
{
	return _mime_types;
}

HttpResponseLine&    Response::getResponseLine() 
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
    return _response;
}

void		Response::initializeMap(std::string file_name)
{
	std::fstream    file;
	std::string		line;

	file.open(file_name, std::ios::in);
    if (file.is_open())
    {
		std::stringstream st;
		std::string key;
		std::string value;

        while (getline(file, line))
        {
			st << line;

			st >> key;
			st >> value;

			_mime_types[key] = value;
		}
	}
	file.close();
}

ServerConfig&     Response::getMacthedServer(std::string& host)
{
    for (size_t i = 0; i < _config_vec.size() ; i++)
    {
        // you have to compare it to all host value
        if (host == _config_vec[i].get_dirs()["listen"].front())
        {
            std::cout << "config host : " << host << std::endl;
            return _config_vec[i];
        }
    }
    std::cout << "host :" << host << std::endl;
    throw "HOST VALUE NOT MATCHED";
}

void     Response::getMacthedLocation(std::string path)
{
    if (path.rfind("/") == std::string::npos)
        return ;
    // if (path.rfind("/") == 0)
    //     return ;
    for (size_t i = 0; i < _locations.size() ; i++)
    {
        if (path == _locations[i].first)
        {
            _location = path;
            location_dirs = _locations[i].second;
        }
    }
    getMacthedLocation(path.substr(0, path.rfind("/")));
}

void        Response::setConfig(std::vector<ServerConfig>& config_vec)
{
    _config_vec = config_vec;
}

void    Response::set_location_vars()
{
    std::string str;

    str = "off";
    if (location_dirs.find("root") != location_dirs.end())
		_conf.setRoot(location_dirs["root"].front());
	if (location_dirs.find("redirection") != location_dirs.end())
		_conf.setRedirection(location_dirs["redirection"].front());
    if (location_dirs.find("redirection") == location_dirs.end())
		_conf.setRedirection(str);
	if (location_dirs.find("autoindex") != location_dirs.end())
		_conf.setAutoIndex(location_dirs["autoindex"].front());
    if (location_dirs.find("autoindex") == location_dirs.end())
        _conf.setAutoIndex(str);
	if (location_dirs.find("client_max_body_size") != location_dirs.end())
		_conf.setClientMaxBodySize(stol(location_dirs["client_max_body_size"].front()));
	if (location_dirs.find("error_log") != location_dirs.end())
		_conf.setErrorLog(location_dirs["error_log"].front());
	if (location_dirs.find("access_log") != location_dirs.end())
		_conf.setAccessLog(location_dirs["access_log"].front());
    if (location_dirs.find("index") != location_dirs.end())
		_conf.setIndex(location_dirs["index"].front());
    if (location_dirs.find("alias") != location_dirs.end())
		_conf.setAlias(location_dirs["alias"].front());
    if (location_dirs.find("alias") == location_dirs.end())
		_conf.setAlias(str);
}

void                Response::checkHttpVersion(const std::string& http_version)
{
    if (std::find(_http_versions.begin(), _http_versions.end(), http_version) != _http_versions.end())
        throw "HTTP/1.1 505 HTTP Version Not Supported";
    else if (http_version != "HTTP/1.1" && std::find(_http_versions.begin(), _http_versions.end(), http_version) == _http_versions.end())
        throw "HTTP/1.1 400 Bad Request";
}

void                Response::checkMethodValidity(const std::string& method, std::vector<std::string>& methods)
{
    int     i;

    if (std::find(_http_methods.begin(), _http_methods.end(), method) == _http_methods.end())
        throw "HTTP/1.1 400 Bad Request";
    if (!method.empty())
    {
        std::cout << "METHODS = > ";
        for (i = 0; i < methods.size(); i++)
        {
            std::cout << methods[i] << " ";
            if (method == methods[i])
                break ;
        }
        std::cout << std::endl;
        if (i == methods.size())
            throw "HTTP/1.1 405 Method Not Allowed";
    }
    else 
        throw "HTTP/1.1 501 Not Implemented";
}


void        Response::checkBodySize(const unsigned long& body_size, int client_max_body_size)
{
    std::cout << "body size : " << body_size << " " << "client_max_body_size : " << client_max_body_size << std::endl;
    if (body_size > client_max_body_size)
        throw "413 Request Entity Too Large";
}

void       Response::isLocationHaveRedirection()
{
    DIRS_PAIR::iterator it = location_dirs.find("return");

    if (it != location_dirs.end())
    {
        _response_header.setLocation(it->second.back());
        throw "301 Moved Permanently";
    }
    
}

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
    return -1;
}

int     Response::isDirHasIndexFiles(std::string path)
{
    
    path += _conf.getIndex();

    if (access(path.c_str(), F_OK) == -1)
    {
        std::cout << "LOCATION PATH : " << path << " NOT FOUND IN DISQUE" << std::endl;
        return (0);
    }
    return (1);
}
int     Response::isUriHasSlashInend(const std::string& path)
{
    if (path[path.length() - 1] != '/')
    {
        std::string location = path;
        location += "/";
        _response_header.setLocation(location);
        std::cout << "URI DOESNT HAVE / AT END" << std::endl;
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
std::string     Response::getIndex()
{
    if (_conf.get_dirs().find("autoindex") != _conf.get_dirs().end())
    {
        return (_conf.get_dirs()["autoindex"].front());
    }
    return ("");
}
//|=======================|

void       Response::handleGetMethod()
{
    std::string path;

    path = _conf.getRoot();
    path += _request.get_request_line().getPath();

   

    std::cout << "PATH => " << path << std::endl;
    // if (!checkPathInRoot(path))
    //     throw "HTTP/1.1 404 Not Found";
    if (getResourceType(path) == DIR)
    {
        std::cout << "MATCHED LOCATION : |" << _location  << "|" << std::endl;

        if (_conf.getAlias() != "off")
        {
            path = _conf.getAlias();
            int needle;
            std::string haystack;

            needle = _request.get_request_line().getPath().find(_location); 
            needle += _location.length();
            haystack = _request.get_request_line().getPath();
            
            std::string ndl =  haystack.substr(needle, _location.length()) ;
            std::cout << "absolute path :" << ndl << std::endl;
            path += ndl;
            std::cout << "alias : " << path << std::endl;

        }
        if (!isUriHasSlashInend(_request.get_request_line().getPath()))
            throw "301 Moved Permanently";
        else if (!isDirHasIndexFiles(path))
        {
            std::cout << "INDEX NOT FOUND" << std::endl;
            if (getAutoIndex() == OFF)
                throw "HTTP/1.1 403 Forbidden";
            else 
            {
                std::cout << "AUTO INDEX ON" << std::endl;
                // _conf.
                std::cout << "INDEX VALUE " <<  _conf.getIndex() << std::endl;
                path += _conf.getIndex();

                _body = AutoIndex::getContentPage(_request.get_request_line().getPath() + _conf.getIndex(), path);
                // std::cout << "|================BODY================|" << std::endl;
                // std::cout << _body << std::endl;
                // std::cout << "|====================================|" << std::endl;
            }
        }
        else 
        {
            std::cout << "ABSOLUTE PATH " << path + _conf.getIndex() << std::endl;
            path += _conf.getIndex();
            _body = AutoIndex::getContentPage(_request.get_request_line().getPath() + _conf.getIndex(), path);
        }
    }
    else
    {
        std::cout  << "SERVED A FILE" << std::endl;
        std::cout << "ABSOLUTE PATH : " << path << std::endl;
        
        _body = AutoIndex::getContentPage(_request.get_request_line().getPath(), path);
    }
    

}

void    Response::createResponse(std::string& path)
{
    std::string ext;
    
    if (path.find(".") != std::string::npos)
    {
        ext = path.substr(path.find(".") + 1, (path.length() - path.find(".")));
        ext = Response::_mime_types[ext];
    }
    else 
        ext = "text/html";
    _response_header.setContentType(ext);

    if (_body.empty())
    {
        _body =     "<!DOCTYPE html>\n" 
                    "<html lang=\"en\">\n" 
                    "<head>\n" 
                    "    <meta charset=\"UTF-8\">\n"
                    "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
                    "    <title>" + _response_line.getStatus() + " " + _response_line.getMessage() + "</title>\n"
                    "</head>\n"
                    "<body>\n"
                    "    <h1>" +  _response_line.getStatus() + " " + _response_line.getMessage() + "</h1>\n"
                    "</body>\n"
                    "</html>";
    }

    _response_header.setContentLength(std::to_string(_body.length()));

    _response = _response_line.getHttpVersion() + " " + _response_line.getStatus() + " " + _response_line.getMessage() + "\r\n" +
    // "Content-Type: " + _response_header.getContentType() + "\r\n" +
    "Content-Type: " + _response_header.getContentType() + "\r\n" +
    "Content-Length: " + _response_header.getContentLength() + "\r\n" +
    "Location: " + _response_header.getLocation() + "\r\n" +
    // "Location: " + "" + "\r\n" +
    "\r\n" + _body;

}

void    Response::handleRequest(int fd)
{
    std::string path_value;
    std::stringstream ss;
    std::string path;
    std::string response;
    std::string method;

    _conf = getMacthedServer( _request.get_request_header().get_directives()["Host"]);
    path = _request.get_request_line().getPath();
    std::cout << "PATH VALUE : " << path << std::endl;

    try 
    {
        _request.isReqWellFormed(fd);
        checkBodySize(_request.get_body().length(), _conf.getClientMaxBodySize());
        _locations = _conf.get_locations();
        path_value = _request.get_request_line().getPath();
        checkHttpVersion(_request.get_request_line().getHttpVersion());
        getMacthedLocation(path_value);
        method = _request.get_request_line().getMethod();

        if (!location_dirs.empty())
        {
            std::cout << "MATCHED LOCATION" << std::endl;
            set_location_vars();
            std::cout << "method : |" << method << "|" << std::endl;
        }   
        else 
        {
            std::cout << "NO MATCHED LOCATION" << std::endl;
            location_dirs = _conf.get_dirs();
        }
        path_value = _conf.getRoot();
        path_value += _request.get_request_line().getPath();

        isLocationHaveRedirection();

        if (location_dirs.find("allowed_methods") != location_dirs.end())
            checkMethodValidity(method, location_dirs["allowed_methods"]);

        //  CHECK PATH IN ROOT
        std::cout << "P VAL : " << path_value << std::endl;
        if (!checkPathInRoot(path_value))
        {
            std::cout << "CHECK IN ROOT" << std::endl;
            throw "404 Not Found";
        }
        // |=================|
        


        // parse mime types file
        // read from a file all contnet
        // set content-type depends on mime type file
        

        // //check if allowed_method dir inherit its value

      
        if (method == "GET")
            handleGetMethod();        
    }
    catch (const char * excep) 
    {
        std::cout << "response exception : " << excep << std::endl;
        std::string exp = excep;
        _response_line.setStatus(exp.substr(0, exp.find(" ")));
        _response_line.setMessage(exp.substr(exp.find(" ") + 1, exp.length() - _response_line.getStatus().length()));
       
    }
    createResponse(path);
}












