
#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <map>
#include "request.hpp"
#include "response_line.hpp"
#include "response_header.hpp"
#include <iostream>
#include <string.h>
#include <sstream>
#include <vector>
#include "config_file.hpp"

#define DIR 0
#define FILE 0

class Response
{
    public :
        Response();
        Response(Request);
        
        const HttpResponseLine&     getResponseLine() const;
        const HttpResponseHeader&   getResponseHeader() const;
        const std::string&          getBody() const;

        const std::string&          getResponse() const;

        ServerConfig&               getMacthedServer(std::string&);
        DIRS_PAIR                   getMacthedLocation(std::string);
        
        void                        handleRequest(int fd);
        void                        setConfig(std::vector<ServerConfig>&);


        void                        set_location_vars();

        //|=====Errors check======|
        void                         is_location_have_redirection();
        void                         checkHttpVersion(const std::string&);
        void                         checkMethodValidity(const std::string&,std::vector<std::string>&);
        // int                         isUriTooLong(int&);
        void                         checkBodySize(const unsigned long&, int);

        void                         handleGetMethod();
        int                         get_resource_type(std::string&);

        //|=======================|
    private :


        Request             _request;
        HttpResponseLine    _response_line;
        HttpResponseHeader  _response_header;
        std::string         _body;
        std::string         _response;
        std::vector<ServerConfig> _config_vec;
        ServerConfig        _conf;
        std::vector<LOCATION_PAIR >   _locations;
        DIRS_PAIR location_dirs;

        DIRS_PAIR       _directives;
        std::vector<std::string> _http_versions;
        std::vector<std::string> _http_methods;
};

#endif