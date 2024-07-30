
#include <string>
#include <map>
#include <unistd.h>
#include <fcntl.h>
#include <fstream>

class Cgi
{
    public :
        std::string     getRequestMethod();
        std::string     getContentType();
        int             getContentLength();
        std::map<std::string, std::string > getHttpCookies();
        std::string     getQueryParams();
        std::string     getScriptName();
        std::string     getBin();

        void            setRequestMethod(std::string);
        void            setContentType(std::string);
        void            setContentLength(int);
        void            setHttpCookies(std::map<std::string, std::string >);
        void            setQueryParams(std::string);
        void            setScriptName(std::string);
        void            setEnv(char **env);
        void            setBin(std::string);
        void            execute();
        std::string     getBody();

    private :

        std::string     _request_method;
        std::string     _content_type;
        int             _content_length;
        std::map<std::string, std::string > _http_cookies;
        std::string     _query_params;
        std::string     _script_name;
        char            **_env;
        std::string     _body;
        std::string     _bin;
};