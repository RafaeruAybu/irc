#include "../includes/User.hpp"

User::User() : user_name("Undefined"), nick("Undefined"), f_logged(0), f_operator(0) {}

User::User(std::string name, std::string nick) : user_name(name), nick(nick)
    , f_logged(0), f_operator(0) {}

User::User(const User &other) : user_name(other.user_name), nick(other.nick)
    , f_logged(other.f_logged), f_operator(other.f_operator) {}

User::~User() {}

User & User::operator= (const User &other)
{
    if (this == &other)
        return *this;

    user_name = other.user_name;
    nick = other.nick;
    f_logged = other.f_logged;
    f_operator = other.f_operator;
    return *this;
}
