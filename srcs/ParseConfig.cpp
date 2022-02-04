#include <string>
#include <fstream>
#include <iostream>
#include "../includes/Server.hpp"

Config Serv::ParseConfig(char *argv[])
{
	Config config;
	config.port = std::string(argv[1]); //convert cstyle to std::string
	config.password = std::string(argv[2]); //same
	std::ifstream file("config");
	std::string str;
	std::getline(file, config.server_name);
	std::getline(file, config.operator_password);
	if (std::getline(file, config.operator_password))
		throw std::invalid_argument( "Config must only contain two lines: Server name and Operator Password!");
	file.close();
	return config;
}