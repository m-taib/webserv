#include "../includes/config_file.hpp"
#include <_types/_intmax_t.h>
#include <codecvt>
#include <cstdarg>
#include <cstring>
#include <iostream>
#include <string>
#include <sys/_types/_size_t.h>
#include <utility>
#include <stdlib.h>
#include <vector>

ServerConfig::ServerConfig()
{
	
}

ServerConfig::ServerConfig(const ServerConfig& rhs)
{
	this->_dirs = rhs._dirs;
	this->_locations = rhs._locations;
}

DIRS_PAIR&  ServerConfig::get_dirs() 
{
	return _dirs;
}

ServerConfig::ServerConfig(std::fstream& file)
{
	std::string     line;
	std::string 	token;
	std::string 	key;
	std::string 	value;
	int             cr_count = 0;
	
	while (getline(file, line))
	{
		while (line.find('\t') != std::string::npos)
			line = strtok((char *)line.c_str(), "\t");

		std::stringstream s(line);

		// std::cout << line << std::endl;
		if (line == "{" || line == "}")
		{
			if (line == "{")
				cr_count -= 1;
			else if (line == "}")
				cr_count += 1;
		}
		else if (line.find("location") != std::string::npos || line.find("cgi" ) != std::string::npos)  
		{
			std::stringstream st(line);
			std::string location;

			// std::cout << "---------INSIDE LOCATION----------" << std::endl;
			st >> location;
			st >> location;
			std::map<std::string, std::vector<std::string> > dirs;
			while (getline(file, line))
			{
				while (line.find('\t') != std::string::npos)
					line = strtok((char *)line.c_str(), "\t");
				if (line == "{")
					cr_count -= 1;
				else if (line == "}")
				{
					cr_count += 1;
					break;
				}
				else
				{
					std::stringstream st(line);

					st >> key;
					while (st >> value)
					{
						if (value.find(';') != std::string::npos)
							value =  value.substr(0, value.find(';'));
						dirs[key].push_back(value);
					}
				}
			}
			
			// std::cout << "KEY => " << location <<  std::endl;
			_locations.push_back(std::make_pair(location, dirs));
			DIRS_PAIR::iterator     it;

			// std::cout << "PRINT DIRECTIVES" << std::endl;
			for ( it = dirs.begin(); it != dirs.end(); it++)
			{
				// std::cout << it->first << " => " ;
				for (size_t i = 0; i < it->second.size() ; i++)
				{
					// std::cout << it->second[i] << " ";
				}
				// std::cout << std::endl;
			}
			// std::cout << "--------------------" << std::endl;
			// std::cout << "---------END LOCATION----------" << std::endl;
		}
		else
		{
			s >> key;
			// std::cout << key << " => " ;
			std::string token;
			while (s >> value)
			{
				token =  value.substr(0, value.find(';'));
				// std::cout << token << " ";
				_dirs[key].push_back(token);
			}
			// std::cout << std::endl;
			

		}
		if (!cr_count)
			break ;
	}
	// DIRS_PAIR::iterator     it;
	// std::vector<DIRS_PAIR > m;

	// std::cout << "PRINT DIRECTIVES" << std::endl;
	// for ( it = _dirs.begin(); it != _dirs.end(); it++)
	// {
	// 	std::cout << it->first << " => " ;
	// 	for (int i = 0; i < it->second.size() ; i++)
	// 	{
	// 		std::cout << it->second[i] << " ";
	// 	}
	// 	std::cout << std::endl;
	// }
	// std::cout << "----------------" << std::endl;

	
}

std::vector<std::string>    ServerConfig::split_string(std::string& dirs, std::string del)
{	
	std::vector<std::string > dirs_Vec;
	char *	token;

	token = strtok((char *)dirs.c_str(), del.c_str());

	while (token != NULL)
	{
		dirs_Vec.push_back(token);
		token = strtok(NULL, " , ");
	}
	return dirs_Vec;
}

void      ServerConfig::extract_data()
{
	std::string dirs = "index root redirection autoindex client_max_body_size error_log access_log error_pages allowed_methods mimeTypes";
	std::vector<std::string > dirs_Vec;

	dirs_Vec = split_string(dirs, " ");
	// check if directive is supported
	_root = "/Users/mtaib/Desktop/sockets-programing";
	_index = "index.html";
	_autoindex = "off";

	
	if (_dirs.find("root") != _dirs.end())
		_root = _dirs["root"].front();
	if (_dirs.find("redirection") != _dirs.end())
		_redirection = _dirs["redirection"].front();
	if (_dirs.find("autoindex") != _dirs.end())
		_autoindex = _dirs["autoindex"].front();
	if (_dirs.find("client_max_body_size") != _dirs.end())
		_client_max_body_size = stol(_dirs["client_max_body_size"].front());
	if (_dirs.find("error_log") != _dirs.end())
		_error_log = _dirs["error_log"].front();
	if (_dirs.find("access_log") != _dirs.end())
		_access_log = _dirs["access_log"].front();
	if (_dirs.find("index") != _dirs.end())
		_index = _dirs["index"].front();
	if (_dirs.find("alias") != _dirs.end())
		_alias = _dirs["alias"].front();
}

std::vector<LOCATION_PAIR > ServerConfig::get_locations() const
{
	return _locations;
}