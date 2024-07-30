
#ifndef HTTPREQUESTHEADER_HPP
#define HTTPREQUESTHEADER_HPP

#include <string>
#include <map>


class HttpRequestHeader
{
    public :

        HttpRequestHeader();
        std::map<std::string, std::string> get_directives() const;
        void     set_directives(std::string, std::string);
        int      get_req_state() const;
        void     set_req_state(int state);

    private :

        std::map<std::string, std::string> _directives;
        int     req_state;

};

#endif