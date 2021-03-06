#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <cctype>
#include <ctime>
#include "Request.hpp"

class Serv;

class User {
public:
    User(int fd);
    User(const User &other);
    ~User();

    User & operator= (const User &other);

    int getFdUser();
    const std::string &getNickUser();
    const std::string &getUserUser();
    int getFlagReg();
    int getFlagOper();

    void setNick(std::string nick);
    void setFlagReg();
    void setFlagOper();
    void setUserUser(std::vector<std::string> tmp_usr);

    void sendMTD();
    void sendSTDReplay(std::string code, std::string text);
    int sendPrivMSG(Request comm_exmpl, std::string sender);
    void setTimeStamp(std::time_t newTimeStamp);
    std::time_t getTimeStamp();

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
    std::time_t _timestamp;

    bool f_logged;
    bool f_operator;
};

#endif
