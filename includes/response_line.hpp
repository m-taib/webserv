#ifndef HTTPRESPONSELINE_HPP
#define HTTPRESPONSELINE_HPP

#include <string>

class HttpResponseLine
{
    public :

        HttpResponseLine();
        // ~HttpRequestLine();
        const std::string& getHttpVersion() const;
        const std::string& getStatus() const;
        const std::string& getMessage() const;

        void     setHttpVersion(std::string);
        void     setStatus(std::string);
        void     setMessage(std::string);
    private :

        std::string _http_version;
        std::string _status;
        std::string _message;
};

#endif