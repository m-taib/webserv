#ifndef CONFIG_FILE_HPP
#define CONFIG_FILE_HPP
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <utility>
#include <fstream>
#include <sstream>
#include "CommonDirectives.hpp"

typedef std::map<std::string, std::vector<std::string> > DIRS_PAIR;

typedef std::pair<std::string, DIRS_PAIR > LOCATION_PAIR;

class ServerConfig : public CommonDirectives
{
    private :

        DIRS_PAIR _dirs;
        std::vector<LOCATION_PAIR >  _locations;
        std::vector<std::string>    allowed_dirs;

    public :

        ServerConfig();
        ServerConfig(std::fstream& file);
        ServerConfig(const ServerConfig& rhs);

        DIRS_PAIR& get_dirs() ;

        std::map<std::string, std::string > getMimeTypes() const;
        std::vector<LOCATION_PAIR > get_locations() const;
        void      extract_data();
        std::vector<std::string>    split_string(std::string&, std::string);
};

#endif