#include "../includes/User.hpp"

User::User() : _user_name("Undefined"), _nick("Undefined"), f_logged(0), f_operator(0), _fd(0), _flag_reg(0), _host_name(""), _servername(""), _realname("")   {}

User::User(int fd) : _user_name("Undefined"), _nick("Undefined"), f_logged(0), f_operator(0), _fd(fd), _flag_reg(0), _host_name(""), _servername(""), _realname("")  {}

//User::User(std::string name, std::string nick) : _user_name(name), _nick(nick), f_logged(0), f_operator(0) {}

User::User(const User &other) : _user_name(other._user_name), _nick(other._nick)
    , f_logged(other.f_logged), f_operator(other.f_operator) {}

User::~User() {}

User & User::operator= (const User &other)
{
    if (this == &other)
        return *this;

    _user_name = other._user_name;
    _nick = other._nick;
    f_logged = other.f_logged;
    f_operator = other.f_operator;
    return *this;
}

const int User::getFdUser() {
    return (_fd);
}

const std::string &User::getNickUser() {
    return (_nick);
}

const std::string &User::getUserUser() {
    return (_user_name);
}

void User::setNick(std::string nick){
    _nick = nick;
}

void User::setFlagReg() {
    _flag_reg = 1;
}

void User::setUserUser(std::vector<std::string> tmp_usr) {
    _user_name = tmp_usr[0];
    _host_name = tmp_usr[1];
    _servername = tmp_usr[2];
    _realname = tmp_usr[4];
}

const int User::getFlagReg() {
    return (_flag_reg);
}





