#pragma once

#include <string>
#include <vector>

class CommonDirectives
{
    protected:
        std::string                    _root;
        // VecString                      index;
        std::string                    _redirection;
        std::string                    _autoindex;
        long                           _client_max_body_size;
        std::string                    _error_log;
        std::string                    _access_log;
        
    public:
        std::string     getRoot() const;
        std::string     getRedirection() const;
        std::string     getAutoIndex() const;
        std::string     getErrorLog() const;
        std::string     getAccessLog() const;
        long            getClientMaxBodySize() const;

        void            setRoot(std::string&);
        void            setRedirection(std::string&);
        void            setAutoIndex(std::string&);
        void            setErrorLog(std::string&);
        void            setAccessLog(std::string&);
        void            setClientMaxBodySize(long);
        
    // MapStringString                mimeTypes;
    // MapIntString                   error_pages;
    // VecString                      allowed_methods;
};

