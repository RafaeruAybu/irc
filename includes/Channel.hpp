#ifndef CHANNEL_HPP
# define CHANNEL_HPP
#include <vector>
#include <iostream>
#include <string>
#include "User.hpp"
//#include "Server.hpp"

//class Serv;

class Channel
{
public:
    Channel(std::string name, std::vector<User*> *vect_user);
    Channel(const Channel &other);
    ~Channel();

    std::string getNameChannel();
    void addUserChannel(User *new_user);
    User* getUserChannel(std::string const &name_user);
    void sendJoinAll(std::string new_user);

private:
    Channel();
    Channel & operator= (const Channel &other);
    std::string _name;
    std::vector<User*> *_ptr_vector_users; //Весь список юзеров на сервере //Пока не пойму - зачем
    std::vector<User*> _channel_user; // юзеры в канале

    ////utils

};

#endif
