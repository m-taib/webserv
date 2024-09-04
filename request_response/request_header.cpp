#include "../includes/request_header.hpp"

HttpRequestHeader::HttpRequestHeader()
{
    req_state = 0;
}
int      HttpRequestHeader::get_req_state() const
{
    return req_state;
}

void      HttpRequestHeader::set_req_state(int state)
{
    req_state = state;
}


std::map<std::string, std::string>& HttpRequestHeader::get_directives() 
{
    return _directives;
}

void     HttpRequestHeader::set_directives(std::string key, std::string value)
{
    _directives[key] = value;
}

