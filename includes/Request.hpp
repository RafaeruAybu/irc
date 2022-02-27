#ifndef FT_IRC_COMMAND_HPP
# define FT_IRC_COMMAND_HPP

//using namespace std;

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>

class Request{
public:

    Request(const std::string buf);
    Request(const Request &copy);
    ~Request();

    Request &operator= (const Request &assign);

    const std::string get_comm() const;
    const std::vector<std::string>& get_vect_arg() const;
private:
    Request();

    std::string _str_command;
    std::vector<std::string> _vect_args;
};

#endif
