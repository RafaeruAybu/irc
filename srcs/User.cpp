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

int User::getFdUser() {
    return (_fd);
}

const std::string &User::getNickUser() {
    return (_nick);
}

const std::string &User::getUserUser() {
    return (_user_name);
}

void User::setNick(std::string nick){
    _nick = (nick);
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

int User::getFlagReg() {
    return (_flag_reg);
}

void User::sendMTD() {

    std::string mes_375 = ":IRC 375 " + _nick + " :- IRC Message of the day -\r\n";
    std::string mes_372 = ":IRC 372 " + _nick + " :- IRC Welcome to server!!!\r\n";
    std::string mes_376 = ":IRC 376 " + _nick + " :End of /MOTD command\r\n";

//    std::string mess001 = ":001 * Welcome to IRC " + _nick + "!@127.0.0.1\r\n"; //NEED? Есть MTD
//    write(_fd, mess001.c_str(), mess001.length()); //NEED?

    write(_fd, mes_375.c_str(), mes_375.length());
    write(_fd, mes_372.c_str(), mes_372.length());
    write(_fd, mes_376.c_str(), mes_376.length());

}

//void User::sendJoinReplay(std::string channel_name) {
//
//    std::string mes_join = ":" + _nick + " JOIN :" + channel_name + "\r\n";
//
//    std::cout << mes_join;
//
//    write(_fd, mes_join.c_str(), mes_join.length());
//
//
////            :dduck!12@127.0.0.1 JOIN :#chan_kek //:nick JOIN :#channel_name -
//
////            :IRCat 331 dduck #chan_kek :No topic is set //RPL_NOTOPIC
////            :IRCat 353 dduck = #chan_kek :@dduck //RPL_NAMREPLY "<channel> :[[@|+]<nick> [[@|+]<nick> [...]]]"
////            :IRCat 366 dduck #chan_kek :End of /NAMES list //RPL_ENDOFNAMES
//
//}

void User::sendSTDReplay(std::string code, std::string text) {

    std::string response_serv = ":IRC " + code + " " + getNickUser() + " : " + text + "\r\n";

//    response_serv = my_response.code_response + " : " + my_response.str_response + "\r\n";
    write(_fd, response_serv.c_str(), response_serv.length()); // Отправка ответа в сокет
}

int User::sendPrivMSG(Request comm_exmpl, std::string sender) { //нет проверки на ':'
    std::string replay;
    std::string replay2;
    
    size_t size_vect_arg = comm_exmpl.get_vect_arg().size();
    std::vector<std::string> vect_arg;

    vect_arg = comm_exmpl.get_vect_arg();


    replay = ":" + sender + " " + comm_exmpl.get_comm();
//    replay2 = ":IRC 303 " + _nick + " : " + sender + "\r\n"; уже отправляется в sendSTDReplay

    if (!_flag_away) {
        for (size_t i = 0; i < size_vect_arg; i++){
            replay += (" " + vect_arg[i]);
        }
        replay += "\r\n";
//        std::cout << "replay2:" << replay2;
//        std::cout << "replay:" << replay;

//        write(_fd, replay2.c_str(), replay2.length()); // :IRC 303 dduck : oper
        write(_fd, replay.c_str(), replay.length()); // :oper PRIVMSG dduck :opop
        return (1);
    }
    else
        return (0);

    //:IRC 303 oper : dduck
    //dduck!Adium@127.0.0.1 PRIVMSG oper :Tut? Horoshiy chelobek?
    //oper PRIVMSG dduck :Ent
}

int User::getFlagOper(){
    return (_flag_operator);
}

void User::setFlagOper(){
    _flag_operator = 1;
}

void User::setTimeStamp(std::time_t newTimeStamp){
    _timestamp = newTimeStamp;
}


