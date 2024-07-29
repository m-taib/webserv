#include <cstdio>
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

int     main()
{

    // _locations.push_back("/usr/bin");
    // _locations.push_back("/usr/bin/ls");
    // getMacthedLocation("/usr/bin/ls/wc");
    // std::fstream ff;
    // std::string line;
    // getline(ff, line);
    std::vector<std::string > vec;

    vec.push_back("a");
    vec.push_back("b");
    vec.push_back("c");
    vec.clear();
    if (vec.empty())
        std::cout << "empty" << std::endl;
    return (0);
}