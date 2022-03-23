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
    int fd;

    if (_channel_user.size() > 0){
        it_begin = _channel_user.begin();
        it_end = _channel_user.end();
        for (; it_begin != it_end; it_begin++) {
            std::string mes_join_all = ":" + new_user + "!" +(*it_begin)->getUserUser() +"@127.0.0.1 JOIN :" + getNameChannel() + "\r\n";
            fd = (*it_begin)->getFdUser();
            write(fd, mes_join_all.c_str(), mes_join_all.length());
        }
    }
}

void Channel::addUserChannel(User *new_user){  //
    User* tmp = getUserChannel(new_user->getNickUser());

    if (!tmp)
        _channel_user.push_back(new_user); //Добавили указатель в вектор указателей
    else { // ник есть в канале, ничего не делаем
    }
}

std::string Channel::getWhoChannel()
{
	std::string res = "";
	std::vector<User *>::iterator it_begin;
	std::vector<User *>::iterator it_end;
	
	if (_channel_user.size() > 0)
	{
		it_begin = _channel_user.begin();
		it_end = _channel_user.end();
		for (; it_begin != it_end; it_begin++)
			res += (*it_begin)->getNickUser() + " ";
        res.erase(res.end() - 1);
	}
	return (res);
}

std::string Channel::getMessage(std::vector<std::string> vect_arg) {
	
	int flag_mnogo = 0;
	std::string res = "";
	
	if (vect_arg.size() == 1 )
		return (res);
	if (vect_arg.size() > 2) //есть не только #chan_name и первый аргумент
		flag_mnogo = 1; //Нужен чтобы удалить пробел в конце
	if(vect_arg.size() > 1 && vect_arg[1].size() > 0 && vect_arg[1][0] != ':')
		return vect_arg[1]; //если нет ':' возвращаем только первое слово
	else{
		for (size_t i = 1; i < vect_arg.size(); i++)
			res += vect_arg[i] + " ";
		if (flag_mnogo)
			res.erase(res.end() - 1);
		res.erase(res.begin());
		return (res);
	}
}

void Channel::sendPrivChannel(std::vector<std::string> vect_arg, std::string sender){
	std::string mess;
	
	mess = getMessage(vect_arg);
	std::vector<User *>::iterator it_begin;
	std::vector<User *>::iterator it_end;
	
	if (_channel_user.size() > 0){
		it_begin = _channel_user.begin();
		it_end = _channel_user.end();
		for (; it_begin != it_end; it_begin++) {
			if ((*it_begin)->getNickUser() != sender)
			{
				std::string mes_join_all = ":" + sender + " PRIVMSG " + getNameChannel() + " :" + mess + "\r\n";
				write((*it_begin)->getFdUser(), mes_join_all.c_str(), mes_join_all.length());
			}
		}
	}
}


void Channel::sendReplaySenderJoin(std::string nick_sender){
    User *tmp_user;
    std::string str_replay_1;
    std::string str_replay_2;
    std::string str_replay_3;
    int fd;

    tmp_user = getUserChannel(nick_sender);
    if (tmp_user){
        str_replay_1 = ":IRC 331 " + nick_sender + " " + getNameChannel() + " :No topic is set\r\n";
        str_replay_2 = ":IRC 353 " + nick_sender + " = " + getNameChannel() + " :@" + getWhoChannel() + "\r\n";
        str_replay_3 = ":IRC 366 " + nick_sender + " " + getNameChannel() + " :End of /NAMES list\r\n";
        fd = tmp_user->getFdUser();
        write(fd, str_replay_1.c_str(), str_replay_1.length());
        write(fd, str_replay_2.c_str(), str_replay_2.length());
        write(fd, str_replay_3.c_str(), str_replay_3.length());
    }
}

void Channel::eraseUserFromChannel(std::string name_user){
    User *tmp_user = getUserChannel(name_user);
    std::vector<User*>::iterator it_begin;
    std::vector<User*>::iterator it_end;

    if (tmp_user){
        it_begin = _channel_user.begin();
        it_end = _channel_user.end();
            for(; it_begin != it_end; it_begin++){
                if ((*it_begin)->getNickUser() == name_user) {
                    _channel_user.erase(it_begin);
                    break;
                }
            }
    }
}

std::string Channel::getNickOperChannel(){
    if (_channel_user.size() < 1)
        return ("");
    return (_channel_user[0]->getNickUser());
}

std::string Channel::getNumberUsers(){
    size_t i;
    std::string str_res;
    std::stringstream ss;
    for (i = 0; i < _channel_user.size(); i++){
    }
    ss << i;
    str_res = ss.str();
    return (str_res);
}