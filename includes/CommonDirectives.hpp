#pragma once

#include <string>
#include <vector>

class CommonDirectives
{
    protected:
        std::string                    _root;
        // server_name global

        // VecString                      index;
        std::string                     _index;
        std::string                    _redirection; //
        std::string                    _autoindex; //
        std::string                    _alias; //

        // FOR POST
        // upload path :string //
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
        std::string     getIndex() const;
        std::string     getAlias() const;

        void            setRoot(std::string&);
        void            setRedirection(std::string&);
        void            setAutoIndex(std::string&);
        void            setErrorLog(std::string&);
        void            setAccessLog(std::string&);
        void            setClientMaxBodySize(long);
        void            setIndex(std::string& index);
        void            setAlias(std::string&);
        
    // MapStringString                mimeTypes;
    // MapIntString                   error_pages;
    // VecString                      allowed_methods;
};

