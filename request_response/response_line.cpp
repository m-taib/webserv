#include "../includes/response_line.hpp"

HttpResponseLine::HttpResponseLine()
{

}

const std::string& HttpResponseLine::get_method() const
{
    return _method;
}

const std::string& HttpResponseLine::get_path() const
{
    return _path;
}

const std::string& HttpResponseLine::get_http_version() const
{
    return _http_version;
}

void     HttpResponseLine::set_method(std::string method)
{
    _method = method;
}
void     HttpResponseLine::set_path(std::string path)
{
    _path = path;
}

void     HttpResponseLine::set_http_version(std::string http_version)
{
    _http_version = http_version;
}
