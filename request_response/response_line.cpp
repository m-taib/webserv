#include "../includes/response_line.hpp"

HttpResponseLine::HttpResponseLine()
{

}

const std::string& HttpResponseLine::getHttpVersion() const
{
    return _http_version;
}

const std::string& HttpResponseLine::getStatus() const
{
    return _status;
}

const std::string& HttpResponseLine::getMessage() const
{
    return _message;
}

void     HttpResponseLine::setHttpVersion(std::string http_version)
{
    _http_version = http_version;
}
void     HttpResponseLine::setStatus(std::string status)
{
    _status = status;
}

void     HttpResponseLine::setMessage(std::string message)
{
    _message = message;
}
