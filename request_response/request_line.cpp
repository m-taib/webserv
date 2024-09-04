#include "../includes/request_line.hpp"

HttpRequestLine::HttpRequestLine()
{
    _path = "";
}

std::string& HttpRequestLine::getMethod() 
{
    return _method;
}

std::string& HttpRequestLine::getPath() 
{
    return _path;
}

std::string& HttpRequestLine::getQueryParams() 
{
    return _query_params;
}

std::string& HttpRequestLine::getHttpVersion() 
{
    return _http_version;
}

void     HttpRequestLine::setMethod(std::string method)
{
    _method = method;
}
void     HttpRequestLine::setPath(std::string path)
{
    _path = path;
}

void     HttpRequestLine::setQueryParams(std::string query_params)
{
    _query_params = query_params;
}

void     HttpRequestLine::setHttpVersion(std::string http_version)
{
    _http_version = http_version;
}
