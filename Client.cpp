#include "Client.hpp"

Client::Client() : name("Undefined"), nick("Undefined"), f_logged(0), f_operator(0) {}

Client::Client(std::string name, std::string nick) : name(name), nick(nick)
    , f_logged(0), f_operator(0) {}

Client::Client(const Client &other) : name(other.name), nick(other.nick)
    , f_logged(other.f_logged), f_operator(other.f_operator) {}

Client::~Client() {}

Client & Client::operator= (const Client &other)
{
    if (this == &other)
        return *this;
    
    name = other.name;
    nick = other.nick;
    f_logged = other.f_logged;
    f_operator = other.f_operator;
    return *this;
}
