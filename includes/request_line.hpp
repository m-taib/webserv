#ifndef HTTPREQUESTLINE_HPP
#define HTTPREQUESTLINE_HPP

#include <string>

class HttpRequestLine
{
    public :

        HttpRequestLine();
        // ~HttpRequestLine();
        std::string& getMethod();
        std::string& getPath();
        std::string& getHttpVersion();
        std::string& getQueryParams();
        
        void     setMethod(std::string);
        void     setPath(std::string);
        void     setHttpVersion(std::string);
        void     setQueryParams(std::string);

    private :

        std::string _method;
        std::string _path;
        std::string _query_params;
        std::string _http_version;
};

#endif