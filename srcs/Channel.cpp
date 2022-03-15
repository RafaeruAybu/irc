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

void Channel::sendJoinAll(std::string new_user){

    std::vector<User *>::iterator it_begin;
    std::vector<User *>::iterator it_end;

    if (_channel_user.size() > 0){
        it_begin = _channel_user.begin();
        it_end = _channel_user.end();
        for (; it_begin != it_end; it_begin++) {
            std::string mes_join_all = ":" + new_user + " JOIN :" + getNameChannel() + "\r\n";
//			std::cout << "Rassilka: '" << mes_join_all;
            write((*it_begin)->getFdUser(), mes_join_all.c_str(), mes_join_all.length());
        }
    }
    //:kek!Adium@127.0.0.1 JOIN :#chkek - рассылка всем в канале, когда присоединился новый юзер //todo: сделать рассылку всем в Channel._channel_user
    // kek - новый юзер, #chkek - канал
}

void Channel::addUserChannel(User *new_user){  //
    User* tmp = getUserChannel(new_user->getNickUser());

    if (!tmp){

        _channel_user.push_back(new_user); //Добавили указатель в вектор указателей
    }
    else { // ник есть в канале, ничего не делаем
    }
}