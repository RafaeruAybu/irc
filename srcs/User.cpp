#include "../includes/User.hpp"

User::User() : _user_name("Undefined"), _nick("Undefined"), f_logged(0), f_operator(0), _fd(0), _flag_reg(0), _flag_operator(0), _flag_away(0), _host_name(""), _servername(""), _realname("")   {}

User::User(int fd) : _user_name("Undefined"), _nick("Undefined"), f_logged(0), f_operator(0), _fd(fd), _flag_reg(0), _flag_operator(0), _flag_away(0), _host_name(""), _servername(""), _realname("")  {}

//User::User(std::string name, std::string nick) : _user_name(name), _nick(nick), f_logged(0), f_operator(0) {}

User::User(const User &other) : _user_name(other._user_name), _nick(other._nick)
    , f_logged(other.f_logged), f_operator(other.f_operator) {}

User::~User() {
    std::cout << "Destruct User\n";
    _fd = 0;
    _user_name.clear();
    _nick.clear();
    _servername.clear();
}

User & User::operator= (const User &other)
{
    if (this == &other)
        return *this;

    _user_name = other._user_name;
    _nick = other._nick;
    f_logged = other.f_logged;
    f_operator = other.f_operator;
    return *this;
}

const int User::getFdUser() {
    return (_fd);
}

const std::string &User::getNickUser() {
    return (_nick);
}

const std::string &User::getUserUser() {
    return (_user_name);
}

void User::setNick(std::string nick){
    _nick = nick;
}

void User::setFlagReg() {
    _flag_reg = 1;
}

void User::setUserUser(std::vector<std::string> tmp_usr) {
    _user_name = tmp_usr[0];
    _host_name = tmp_usr[1];
    _servername = tmp_usr[2];
    _realname = tmp_usr[4];
}

const int User::getFlagReg() {
    return (_flag_reg);
}

void User::sendMTD() {

    std::string mes_375 = ":IRC 375 " + _nick + " :- IRC Message of the day -\r\n";
    std::string mes_372 = ":IRC 372 " + _nick + " :- IRC Welcome to server!!!\r\n";
    std::string mes_376 = ":IRC 376 " + _nick + " :End of /MOTD command\r\n";

//    std::cout << "fd sendMTD:" << _fd << "\n";

    write(_fd, mes_375.c_str(), mes_375.length());
    write(_fd, mes_372.c_str(), mes_372.length());
    write(_fd, mes_376.c_str(), mes_376.length());
}

void User::sendSTDReplay(std::string code, std::string text) {

    std::string response_serv = ":IRC " + code + " " + getNickUser() + " : " + text + "\r\n";


//    response_serv = my_response.code_response + " : " + my_response.str_response + "\r\n";
//    std::cout << response_serv << "\n";
    write(_fd, response_serv.c_str(), response_serv.length()); // Отправка ответа в сокет
}

int User::sendPrivMSG(Request comm_exmpl, std::string sender) {
    std::string replay;


    size_t size_vect_arg = comm_exmpl.get_vect_arg().size();
    std::vector<std::string> vect_arg;

    vect_arg = comm_exmpl.get_vect_arg();

    replay = sender + " " + comm_exmpl.get_comm();


    if (!_flag_away) {
        for (size_t i = 0; i < size_vect_arg; i++){
            replay += (" " + vect_arg[i]);
        }
        replay += "\n";
        std::cout << "replay:" << replay;
        write(_fd, replay.c_str(), replay.length());
        return (1);
    }
    else
        return (0);
    //dduck!Adium@127.0.0.1 PRIVMSG oper :Tut? Horoshiy chelobek?
    //oper PRIVMSG dduck :Ent
}





