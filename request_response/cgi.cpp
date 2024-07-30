#include "../includes/cgi.hpp"
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


void            Cgi::execute()
{
    std::string exec_path;
    int     pid;
    int     arr[2];
    int     fd;
    std::string filename;

    filename = "output.txt";
    exec_path = "/usr/local/bin/python3";
    if (getRequestMethod() == "POST")
    {
        fd = open(filename.c_str(), O_RDONLY | O_CREAT | O_WRONLY );
        std::ofstream file(filename);
        file << _body;
    }
    if (!access(getScriptName().c_str(), F_OK | X_OK))
	{
		throw "500 INTERNEL SERVER ERROR";
	}
   
    if (pipe(arr) == -1)
        throw "500 INTERNEL SERVER ERROR";
	pid = fork();
	if (pid == -1)
        throw "500 INTERNEL SERVER ERROR"; 
	if (pid == 0)
	{
        if (getRequestMethod() == "GET")
            close(arr[0]);
        else
        {
            if (dup2(fd, 0) == -1)
                throw "500 INTERNEL SERVER ERROR"; 
        }
        if (dup2(arr[1],1) == -1)
            throw "500 INTERNEL SERVER ERROR"; 
        close(arr[1]);
        if (execve(exec_path.c_str(), (char **)getScriptName().c_str(), _env))
        {
            perror("execve");
        }
    }
	else 
    {
        close(arr[0]);
        close(arr[1]);
    }
}

