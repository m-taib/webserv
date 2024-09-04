#include <cstdio>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>

std::vector<std::string> _locations;

void     getMacthedLocation(std::string path)
{
    
    if (path.rfind("/") == std::string::npos)
        return ;
    for (size_t i = 0; i < _locations.size() ; i++)
    {
        if (path == _locations[i])
        {
        std::cout << _locations[i] << std::endl;
        return ;
        }
    }
    int length;
    
    length = path.rfind("/");
    getMacthedLocation(path.substr(0, length));
}
int            hexToInt(const std::string& hexStr) 
{
    return std::stoi(hexStr, nullptr, 16);
}

int     main()
{

    // _locations.push_back("/usr/bin");
    // _locations.push_back("/usr/bin/ls");
    // getMacthedLocation("/usr/bin/ls/wc");
    // std::fstream ff;
    // std::string line;
    // getline(ff, line);
    std::string     filename;
    std::string     token;
    int             chunk_size;
    std::fstream file;
    std::ofstream body;

    file.open("test.txt", std::ios::in);
    body.open("body.txt", std::ios::app);

    char s[1024];   
    std::string tmp;
    tmp = s;
    if (file.is_open() && body.is_open())
    {
        while (1)
        { 
            getline(file, token);
            
            token = token.substr(0, token.find('\r'));
            
            chunk_size = hexToInt(token);
            if (!chunk_size)
                break ;
            char buff[chunk_size + 1];

            memset(buff, 0, chunk_size + 1);
            
            file.read(buff, chunk_size);
            std::cout << buff << std::endl;
            file.seekg(2, std::ios::cur);

            body << buff;
            body.flush();
        }
        body.close();
        file.close();
    }

    return (0);
}