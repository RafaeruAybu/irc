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
//    int fd;
    std::string user_name;
    std::string nick;
    std::string flag;
    bool f_logged;
    bool f_operator;
};

#endif
