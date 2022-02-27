#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <iostream>
#include <string>

class User
{
public:
    User();
    User(int fd);
    User(std::string name, std::string nick);
    User(const User &other);
    ~User();

    User & operator= (const User &other);
    const int get_fd_user();
private:
    int _fd;
    std::string user_name;
    std::string nick;
    std::string flag;
    bool f_logged;
    bool f_operator;

    ////

};

#endif
