#include "../includes/cgi.hpp"
#include <cstdio>
#include <cstring>
#include <string>
#include <sys/fcntl.h>
#include <unistd.h>

std::string     Cgi::getRequestMethod()
{
    return _request_method;
}
std::string     Cgi::getContentType()
{
    return _content_type;
}
int             Cgi::getContentLength()
{
    return _content_length;
}

std::string     Cgi::getQueryParams()
{
    return _query_params;
}

std::map<std::string, std::string > Cgi::getHttpCookies()
{
    return _http_cookies;
}
std::string     Cgi::getScriptName()
{
    return _script_name;
}

void            Cgi::setRequestMethod(std::string method)
{
    _request_method = method;
}
void            Cgi::setContentType(std::string content_type)
{
    _content_type = content_type;
}
void            Cgi::setContentLength(int content_length)
{
    _content_length = content_length;
}

void            Cgi::setQueryParams(std::string query_params)
{
    _query_params = query_params;
}

void            Cgi::setHttpCookies(std::map<std::string, std::string > http_cookies)
{
    _http_cookies = http_cookies;
}

void            Cgi::setScriptName(std::string script_name)
{
    _script_name = script_name;
}
void            Cgi::setEnv(char **env)
{
    _env = env;
}
std::string     Cgi::getBin()
{
    return _bin;
}

void            Cgi::setBin(std::string bin)
{
    _bin = bin;
}

void            Cgi::setBody(std::string body)
{
    _body = body;
}

void            Cgi::execute()
{
    std::string exec_path;
    int         pid;
    int         arr[2];
    int         fd;
    std::string filename;
    char        *argv[3];
    int         status;

    argv[0] = (char *)getBin().c_str();
    argv[1] = (char *)getScriptName().c_str();
    argv[2] = NULL;

    filename = "output.txt";

    if (getRequestMethod() == "POST")
    {
        fd = open(filename.c_str(), O_RDONLY | O_CREAT | O_WRONLY );
        std::ofstream file(filename);
        file << _body;
        std::cout << "BODY : \n" << _body << std::endl;
        exit(0);
    }
    if (access(getScriptName().c_str(), F_OK) == -1)
		throw "500 INTERNEL SERVER ERROR";

    if (pipe(arr) == -1)
        throw "pipe 500 INTERNEL SERVER ERROR";
    int fdd = open("file", O_RDONLY);

    std::cout << "READ FD : " << arr[0] << " WRITE FD : " << arr[1] << std::endl;
    std::cout << "BIN : " << argv[0] << ", SCRIPT : " << argv[1] << std::endl;
	pid = fork();
	if (pid == -1)
        std::exit(1); 
	if (pid == 0)
	{
        // chdir(getScriptName().c_str());
        if (!_body.empty())
        {
            // dup file fd
            if (dup2(arr[0], 0) == -1)
                std::exit(1); 
        }
        if (dup2(arr[1], 1) == -1)
            std::exit(1); 
        close(arr[0]);
        close(arr[1]);
        if (execve(getBin().c_str(), argv, _env) == -1)
            std::exit(1);
    }
	else 
    {
        std::cout << "IN PARENT " << std::endl;
        // int     init_time = ;

        while (1)
        {
            if (waitpid(pid, &status, WNOHANG) > 0)
                break ;
            // check time interval 

        }
        if (status >> 8)
        {
            std::cout << "STATUS CODE : " << status << std::endl;
            throw "500 INTERNAL SERVER ERROR";
        }
        char buffer[1024];

        memset(buffer, 0, 1024);
        int n = read(arr[0], buffer, 1023);
        close(arr[1]);

        std::cout << "BUFFER" << std::endl;
        std::cout << buffer << std::endl;
        close(arr[0]);
        exit(0);
    }
}

