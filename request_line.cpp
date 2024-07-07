#include "request_line.hpp"

HttpRequestLine::HttpRequestLine()
{

}

const std::string& HttpRequestLine::get_method() const
{
    return _method;
}

const std::string& HttpRequestLine::get_path() const
{
    return _path;
}

const std::string& HttpRequestLine::get_query_params() const
{
    return _query_params;
}

const std::string& HttpRequestLine::get_http_version() const
{
    return _http_version;
}

void     HttpRequestLine::set_method(std::string method)
{
    _method = method;
}
void     HttpRequestLine::set_path(std::string path)
{
    _path = path;
}

void     HttpRequestLine::set_query_params(std::string query_params)
{
    _query_params = query_params;
}

void     HttpRequestLine::set_http_version(std::string http_version)
{
    _http_version = http_version;
}
