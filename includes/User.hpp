#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <iostream>
#include <string>

class User
{
public:
    User();
    User(std::string name, std::string nick);
    User(const User &other);
    ~User();

    User & operator= (const User &other);
private:
//    int fd;
    std::string user_name;
    std::string nick;
    std::string flag;
    bool f_logged;
    bool f_operator;
};

#endif
