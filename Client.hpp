#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <iostream>
#include <string>

class Client
{
public:
    Client();
    Client(std::string name, std::string nick);
    Client(const Client &other);
    ~Client();

    Client & operator= (const Client &other);
private:
    std::string name;
    std::string nick;
    bool f_logged;
    bool f_operator;
};

#endif
