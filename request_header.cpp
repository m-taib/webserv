#include "request_header.hpp"

HttpRequestHeader::HttpRequestHeader()
{

}
std::map<std::string, std::string> HttpRequestHeader::get_directives() const
{
    return _directives;
}

void     HttpRequestHeader::set_directives(std::string key, std::string value)
{
    _directives[key] = value;
}

