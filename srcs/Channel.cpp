#include "../includes/Channel.hpp"

//Channel::Channel() {}

Channel::Channel(std::string name, std::vector<User*> *vect_user) : _name(name), _ptr_vector_users(vect_user) {
}

std::string Channel::getNameChannel(){
    return (_name);
}

User* Channel::getUserChannel(std::string const &name_user){ //Поиск по нику в канале
    if (_channel_user.size() == 0)
        return (NULL);
    std::vector<User*>::iterator it_begin = _channel_user.begin();
    std::vector<User*>::iterator it_end = _channel_user.end();

    for (; it_begin != it_end; it_begin++){
        if ((*it_begin)->getNickUser() == name_user)
            return (*it_begin);
    }
    return (NULL);
}

void Channel::addUserChannel(User *new_user){  //
    User* tmp = getUserChannel(new_user->getNickUser());

    if (tmp){

    }
    else { // ника нет в канале
        _channel_user.push_back(new_user); //Добавили указатель в вектор указателей
    }
}