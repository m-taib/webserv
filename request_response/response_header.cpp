#include "../includes/response_header.hpp"

HttpResponseHeader::HttpResponseHeader()
{
    
}
std::string HttpResponseHeader::getContentType() const
{
    return _content_type;
}

std::string HttpResponseHeader::getContentLength() const
{
    return _content_length;;
}

std::string HttpResponseHeader::getLocation() const
{
    return _location;
}

void     HttpResponseHeader::setContentType(std::string& content_type)
{
    _content_type = content_type;
}
void     HttpResponseHeader::setContentLength(std::string& content_length)
{
    _content_length = content_length;
}
void     HttpResponseHeader::setLocation(std::string& location)
{
    _location =location;
}

