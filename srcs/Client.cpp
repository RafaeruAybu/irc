#include "../includes/Client.hpp"

Client::Client() : user_name("Undefined"), nick("Undefined"), f_logged(0), f_operator(0) {}

Client::Client(std::string name, std::string nick) : user_name(name), nick(nick)
    , f_logged(0), f_operator(0) {}

Client::Client(const Client &other) : user_name(other.user_name), nick(other.nick)
    , f_logged(other.f_logged), f_operator(other.f_operator) {}

Client::~Client() {}

Client & Client::operator= (const Client &other)
{
    if (this == &other)
        return *this;

    user_name = other.user_name;
    nick = other.nick;
    f_logged = other.f_logged;
    f_operator = other.f_operator;
    return *this;
}
