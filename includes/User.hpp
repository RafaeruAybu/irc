#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include "Request.hpp"
//#include "Server.hpp"

//response_server;

class Serv;

class User
{
public:
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

    void sendMTD();
    void sendSTDReplay(std::string code, std::string text);
    int sendPrivMSG(Request comm_exmpl, std::string sender);

private:
    User();

    int _fd;
    std::string _user_name;
    std::string _nick;
    std::string _host_name;
    std::string _servername;
    std::string _realname;

    int _flag_reg;
    int _flag_operator;
    int _flag_away;
    std::string flag;
    bool f_logged;
    bool f_operator;

    ////
};

#endif
