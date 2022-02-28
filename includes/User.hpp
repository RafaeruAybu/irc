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

    const int getFdUser();
    const std::string &getNickUser();

    void setNick(std::string nick);


private:
    int _fd;
    std::string user_name;
    std::string _nick;
    std::string flag;
    bool f_logged;
    bool f_operator;

    ////

};

#endif
