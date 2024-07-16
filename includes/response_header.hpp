
#ifndef HTTPRESPONSEHEADER_HPP
#define HTTPRESPONSEHEADER_HPP

#include <string>
#include <map>


class HttpResponseHeader
{
    public :

        HttpResponseHeader();

        std::string getContentType() const;
        std::string getContentLength() const;
        std::string getLocation() const;

        void     setContentType(std::string&);
        void     setContentLength(std::string&);
        void     setLocation(std::string&);

    private :

        std::string     _content_type;
        std::string     _content_length;
        std::string     _location;

};

#endif