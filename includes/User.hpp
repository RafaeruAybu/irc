#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <iostream>
#include <string>
#include <vector>

class User
{
public:
    User();
    User(int fd);
//    User(std::string name, std::string nick);
    User(const User &other);
    ~User();

    User & operator= (const User &other);

    const int getFdUser();
    const std::string &getNickUser();
    const std::string &getUserUser();
    const int getFlagReg();

    void setNick(std::string nick);
    void setFlagReg();
    void setUserUser(std::vector<std::string> tmp_usr);

private:
    int _fd;
    std::string _user_name;
    std::string _nick;
    std::string _host_name;
    std::string _servername;
    std::string _realname;

    int _flag_reg;
    std::string flag;
    bool f_logged;
    bool f_operator;

    ////
};

#endif
