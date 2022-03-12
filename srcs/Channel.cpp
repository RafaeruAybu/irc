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

void Channel::sendPriv(std::vector<std::string> tmp_arg, std::string sender){ //уже знаем, что есть не только PRIVMSG
    std::vector<User *>::iterator it_begin;
    std::vector<User *>::iterator it_end;
    size_t count_message;
    std::string message;

    count_message = tmp_arg.size();


    if (count_message > 2){
        message = getMessage(tmp_arg); //Вернет string с учетом ':', удалив ':'
            if (_channel_user.size() > 0){
                it_begin = _channel_user.begin();
                it_end = _channel_user.end();

                for (; it_begin != it_end; it_begin++) {
                    std::string mes_join_all = ":" + sender + " PRIVMSG :" + message + "\r\n";
                    write((*it_begin)->getFdUser(), mes_join_all.c_str(), mes_join_all.length());
                }
            }

    }


    //:kek!Adium@127.0.0.1 PRIVMSG #chan :Hi, people!
    //:nick_sender PRIVMSG #channel :message
}



////UTILS

std::string Channel::getMessage(std::vector<std::string> vect_arg) {

    int flag_mnogo = 0;
    std::string res = "";

    std::cout << "getMessage[0]: " << vect_arg[0] << "\n";

    if (vect_arg.size() > 1)
        flag_mnogo = 1;

    if(vect_arg[1].size() > 0 && vect_arg[1][0] != ':')
        return vect_arg[1];
    else{
        for (size_t i = 1; i < vect_arg.size(); i++){
            res += vect_arg[i] + " ";
        }
        if (flag_mnogo)
            res.erase(res.end() - 1);
        res.erase(res.begin());
        return (res);
    }
}