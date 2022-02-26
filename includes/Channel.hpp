#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include <vector>
#include <iostream>
#include <string>
#include "User.hpp"

class Channel
{
public:
    Channel(std::string name);
    Channel(const Channel &other);
    ~Channel();

private:
    Channel();
    Channel & operator= (const Channel &other);
    std::string name;
    std::vector<User> users;
};

#endif
