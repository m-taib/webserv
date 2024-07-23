#include "../includes/request_line.hpp"

HttpRequestLine::HttpRequestLine()
{
    _path = "";
}

const std::string& HttpRequestLine::getMethod() const
{
    return _method;
}

const std::string& HttpRequestLine::getPath() const
{
    return _path;
}

const std::string& HttpRequestLine::getQueryParams() const
{
    return _query_params;
}

const std::string& HttpRequestLine::getHttpVersion() const
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
