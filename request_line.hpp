#ifndef HTTPREQUESTLINE_HPP
#define HTTPREQUESTLINE_HPP

#include <string>

class HttpRequestLine
{
    public :

        HttpRequestLine();
        // ~HttpRequestLine();
        const std::string& get_method() const;
        const std::string& get_path() const;
        const std::string& get_query_params() const;
        const std::string& get_http_version() const;

        void     set_method(std::string);
        void     set_path(std::string);
        void     set_query_params(std::string);
        void     set_http_version(std::string);

    private :

        std::string _method;
        std::string _path;
        std::string _query_params;
        std::string _http_version;

};

#endif