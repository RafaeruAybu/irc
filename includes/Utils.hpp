#ifndef IRC_UTILS_HPP
#define IRC_UTILS_HPP
struct Config {
	std::string port;
	std::string password;
	std::string operator_password;
	std::string server_name;
};
Config ParseConfig(char *[]);
#endif //IRC_UTILS_HPP
