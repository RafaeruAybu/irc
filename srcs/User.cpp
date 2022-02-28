#include "../includes/User.hpp"

User::User() : user_name("Undefined"), _nick("Undefined"), f_logged(0), f_operator(0) {}

User::User(int fd) : user_name("Undefined"), _nick("Undefined"), f_logged(0), f_operator(0), _fd(fd)  {}

User::User(std::string name, std::string nick) : user_name(name), _nick(nick)
    ,f_logged(0), f_operator(0) {}

User::User(const User &other) : user_name(other.user_name), _nick(other._nick)
    ,f_logged(other.f_logged), f_operator(other.f_operator) {}

User::~User() {}

User & User::operator= (const User &other)
{
    if (this == &other)
        return *this;

    user_name = other.user_name;
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

void User::setNick(std::string nick){
    _nick = nick;
}



