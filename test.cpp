
#include <string>
#include <vector>


int     getMacthedLocation(std::string path)
{
    
    for (size_t i = 0; i < _locations.size() ; i++)
    {
        if (path == (_locations[i].first.c_str() + 1))
        {
            return _locations[i].second;
        }
    }
    int length;

    if (path.rfind("/") == std::string::npos)
    {
        length = path.length();
    }
    else 
        length = path.rfind("/");
    getMacthedLocation(path.substr(0, length));
    throw "NOT FOUND";
}

int     main()
{
    std::vector<std::string> values;

    values.push_back("/usr/bin");
    values.push_back("/usr/bin/ls");
    values.push_back("/usr");
    getMacthedLocation("/usr/bin/ls", values);
    return (0);
}
