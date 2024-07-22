#include "../includes/CommonDirectives.hpp"

std::string     CommonDirectives::getRoot() const
{
    return _root;
}

std::string     CommonDirectives::getRedirection() const
{
    return _redirection;
}

std::string     CommonDirectives::getAutoIndex() const
{
    return _autoindex;

}

std::string     CommonDirectives::getErrorLog() const
{
    return _error_log;

}

std::string     CommonDirectives::getAccessLog() const
{
    return _access_log;

}

std::string     CommonDirectives::getIndex() const
{
    return _index;

}
long            CommonDirectives::getClientMaxBodySize() const
{
    return _client_max_body_size;
}

void            CommonDirectives::setRoot(std::string& root)
{
    _root = root;
}
void            CommonDirectives::setRedirection(std::string& redirection)
{
    _redirection = redirection;
}
void            CommonDirectives::setAutoIndex(std::string& autoindex)
{
    _autoindex = autoindex;
}
void            CommonDirectives::setErrorLog(std::string& error_log)
{
    _error_log = error_log;
}
void            CommonDirectives::setAccessLog(std::string& access_log)
{
    _access_log = access_log;
}
void            CommonDirectives::setIndex(std::string& index)
{
    _index = index;
}

void            CommonDirectives::setAlias(std::string& alias)
{
    _alias = alias;
}

void            CommonDirectives::setClientMaxBodySize(long client_max_body_size)
{
    _client_max_body_size = client_max_body_size;
}

std::string     CommonDirectives::getAlias() const
{
    return _alias;
}
