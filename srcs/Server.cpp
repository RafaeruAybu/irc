#include "../includes/Server.hpp"
#include "../includes/Request.hpp"

Serv::Serv(char *port, char *password) {
    exit_server = false;
    _str_password = password;
    _oper_user = "dduk";
    _oper_pass = "kudd";
    this->password = atoi(password);
    listen_socket = get_listen_sock(atoi(port));
    num = sizeof(fd_list) / sizeof(fd_list[0]);
    int i = 0;
    for (; i < num; i++) {
        fd_list[i].fd = -1;
        fd_list[i].events = 0;
        fd_list[i].revents = 0;
    }
    i = 0;
    for (; i < num; i++) {
        if (fd_list[i].fd == -1) {
            fd_list[i].fd = listen_socket;
            fd_list[i].events = POLLIN;
            break;
        }
    }
    i = 0;
    for (; i < MAX_USERS; i++) {
        for (int j = 0; j < BUFF_SIZE; j++) {
            bufs[i][j] = 0;
        }
    }
    for (long i = 0; i < MAX_USERS; i++){
        arr_timestamp[i] = -1;
    }
}

Serv::~Serv()
{
}

int Serv::get_listen_sock(int port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
        throw "failed getting sock...";
    int opt = 1;
    if (fcntl(sock, F_SETFL, O_NONBLOCK))
        throw "Could not set non-blocking socket...";
    setsockopt(sock, SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    struct sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = htonl(INADDR_ANY);
    local.sin_port = htons(port);
    if (bind(sock,(struct sockaddr *)&local, sizeof(local)) < 0)
        throw "bind failed...";
    if (listen(sock, 5) < 0)
        throw "listen failed";
    return sock;
}

void Serv::do_poll_timeout() {
    std::vector<User*>::iterator it_begin;
    std::vector<User*>::iterator it_end;
    std::time_t realTime;
    User* tmp_user_for_del;
    User* tmp_user_for_fd;
    std::string ping_str("PING 1648063017\r\n");
    for (int i = 1; i < MAX_USERS; i++) {
        if (arr_timestamp[i] != -1) {
            write(fd_list[i].fd, ping_str.c_str(), ping_str.length());
            realTime = std::time(NULL);
            if (realTime - arr_timestamp[i] >= TIMEOUT) {
                tmp_user_for_fd = getUser(fd_list[i].fd);
                if (tmp_user_for_fd) {
                    tmp_user_for_del = *(getUserIter(fd_list[i].fd));
                    if (getUserIter(fd_list[i].fd) != _users.end()) {
                        _users.erase((getUserIter(fd_list[i].fd)));
                        clearChannel(tmp_user_for_del->getNickUser());
                        if (tmp_user_for_del)
                            delete tmp_user_for_del;
                    }
                }
                if (fd_list[i].fd != -1) {
                    close(fd_list[i].fd);
                }
                std::cout << "close_fd[" << fd_list[i].fd << "]\n";
                fd_list[i].fd = -1;
                arr_timestamp[i] = -1;
            }
        }
    }
}

void Serv::do_poll_fail() {
    std::cout << "poll fail..." << std::endl;
}

int Serv::get_new_connection() {
    struct sockaddr_in client;
    socklen_t len = sizeof(client);
    int new_sock = accept(listen_socket,(struct sockaddr *)&client,&len);
    if (new_sock < 0) {
        if (errno != EWOULDBLOCK)
            std::cout << "accept fail..." << std::endl;
        return -1;
    }
    if (fcntl(new_sock, F_SETFL, O_NONBLOCK))
        throw "Could not set non-blocking socket...";
    int i = 0;
    for (; i < num; i++) {
        if (fd_list[i].fd == -1)
            break;
    }
    if (i < num) {
        fd_list[i].fd = new_sock;
        fd_list[i].events = POLLIN;
        arr_timestamp[i] = std::time(NULL);
    }
    else
        close(new_sock);
    std::cout
        << "get a new connection[" << inet_ntoa(client.sin_addr)
        << ":" << ntohs(client.sin_port) << "]" << std::endl;
    return 0;
}

void Serv::do_poll_default() {
    User* tmp_user_for_del;
    int i = 0;
    for (; i < num; i++) {
        if (fd_list[i].fd == -1)
            continue;
        if (fd_list[i].fd == listen_socket && (fd_list[i].revents & POLLIN)) {
            if (get_new_connection() != 0)
                break ;
            else
                continue ;
        }
        if (fd_list[i].revents & POLLIN) {
            char buf[BUFF_SIZE];
            ssize_t s = read(fd_list[i].fd, buf, sizeof(buf)-1);
            if (s < 0) {
                if (errno != EWOULDBLOCK)
                    std::cout << "read failed..." << std::endl;
                break;
            }
            else if (s == 0) {
                std::cout << "client quit..." << std::endl;
                if (_users.size() > 0) {
                    tmp_user_for_del = *(getUserIter(fd_list[i].fd));
                    if (getUserIter(fd_list[i].fd) != _users.end()) {

                        _users.erase((getUserIter(fd_list[i].fd)));
                        clearChannel(tmp_user_for_del->getNickUser());
                        if (tmp_user_for_del)
                            delete tmp_user_for_del;
                    }
                }
                if (fd_list[i].fd != -1)
                    close(fd_list[i].fd);
                fd_list[i].fd = -1;
                break;
            }
            else
                buf[s] = 0;
            process(fd_list[i].fd, (char *) &buf, i);
        }
    }
}

void Serv::get_into_loop() {
    while (!exit_server) {
        switch(poll(fd_list, num, POLL_TIMEOUT)) {
            case 0:
                do_poll_timeout();
                continue ;
            case -1:
                do_poll_fail();
                continue ;
            default:
                do_poll_default();
                break ;
        }
    }
    return ;
}

void Serv::process(int fd, char *buf, int index_fd) {
    response_server my_response;
    Request *command_exmpl;
    User* usr_exmpl = getUser(fd);
    int count_command;
    std::string tmp_buf;
    std::string response_serv = "";
    std::cout << "User[" << fd << "]: " << buf << std::endl;
    count_command = getCountCommand(buf);
    for (int i = 0; i < count_command; i++) {
        tmp_buf = getTmpBuf(i, buf);
        usr_exmpl = getUser(fd);
        command_exmpl = new Request(tmp_buf);
        if (command_exmpl->get_comm() == "PASS")
            my_response = pass(fd, *command_exmpl, usr_exmpl);
        else if (command_exmpl->get_comm() == "USER") {
            my_response = user(fd, *command_exmpl, usr_exmpl);
        } else if (command_exmpl->get_comm() == "NICK")
            my_response = nick(fd, *command_exmpl, usr_exmpl);
        else if (command_exmpl->get_comm() == "PONG") {
            my_response = pongClient(*command_exmpl, usr_exmpl, index_fd);
        }
        if (usr_exmpl) {
            if (command_exmpl->get_comm() == "PRIVMSG") {
                my_response = privmsg(*command_exmpl, usr_exmpl);
            }
            else if(command_exmpl->get_comm() == "NOTICE"){
                my_response = notice(*command_exmpl, usr_exmpl);
            }
            else if (command_exmpl->get_comm() == "JOIN") {
                my_response = join(fd, *command_exmpl, usr_exmpl);
            }
            else if (command_exmpl->get_comm() == "OPER") {
                my_response = oper( *command_exmpl, usr_exmpl);
            }
            else if (command_exmpl->get_comm() == "QUIT") {
                my_response = quit(*command_exmpl, usr_exmpl, index_fd);
            }
            else if (command_exmpl->get_comm() == "KILL") {
                my_response = kill(*command_exmpl, usr_exmpl);
            }
            else if (command_exmpl->get_comm() == "KICK") {
                my_response = kick(*command_exmpl, usr_exmpl);
            }

            else if (command_exmpl->get_comm() == "PING") {
                my_response = pingClient(fd, *command_exmpl);
            }
            else if (command_exmpl->get_comm() == "LIST") {
                my_response = list(usr_exmpl);
            }
			else if (command_exmpl->get_comm() == "WHO") {
				my_response = who(*command_exmpl);
			}
        }
        if (my_response.code_response.length() != 0)
            usr_exmpl->sendSTDReplay(my_response.code_response, my_response.str_response);
        delete command_exmpl;
        my_response.code_response.clear();
        my_response.str_response.clear();
        response_serv.clear();
    }
}

response_server Serv::pass(int fd_client, Request comm_exmpl, User *usr_exmpl) {
	response_server res;
    std::vector<std::string> tmp_arg = comm_exmpl.get_vect_arg();
    if (((tmp_arg.size() != 0) && tmp_arg[0].size() != 0) && (tmp_arg[0][0] == ':'))
        tmp_arg[0].erase(tmp_arg[0].begin());
    if (tmp_arg.size() == 0){
        sendNoUser(fd_client, "461", "ERR_NEEDMOREPARAMS");
    }
    else if (usr_exmpl && usr_exmpl->getFlagReg()){
        res.code_response = "462";
        res.str_response = "PASS ERR_ALREADYREGISTRED";
    }
    else if (tmp_arg[0] == _str_password || _str_password.length() == 0) {
        _users.push_back(new User(fd_client));
    }
	return (res);
}

response_server Serv::nick(int fd_client, Request comm_exmpl, User *usr_exmpl) {
    response_server res;
    std::vector<std::string> tmp_arg = comm_exmpl.get_vect_arg();
    int check_res = 0;
    if (tmp_arg.size() != 0)
        check_res = checkNick(toLowerString(tmp_arg[0]));
    if (!usr_exmpl){
        sendNoUser(fd_client, "451", "ERR_NOTREGISTERED");
    }
    else if (!check_res){
        sendNoUser(fd_client, "432", "ERR_ERRONEUSNICKNAME");
    }
    else if (check_res == 1){
        sendNoUser(fd_client, "433", "ERR_NICKNAMEINUSE");
    }
    else{
        usr_exmpl->setNick(toLowerString(tmp_arg[0]));
        if (usr_exmpl->getUserUser() != "Undefined") {
            usr_exmpl->setFlagReg();
            usr_exmpl->sendMTD();
        }
    }
    return (res);
}

response_server Serv::user(int fd_client, Request comm_exmpl, User *usr_exmpl) {
    response_server res;
    std::vector<std::string> tmp_arg = comm_exmpl.get_vect_arg();
    int count_arg = tmp_arg.size();
    std::vector<std::string> tmp_usr;
    std::string tmp_realname = "";
    if (!usr_exmpl){
        sendNoUser(fd_client, "451", "ERR_NOTREGISTERED");
    }
    else if (((count_arg >= 4) && (tmp_arg[3][0] != ':')) || (count_arg < 4)){
        res.code_response = "461";
        res.str_response = "ERR_NEEDMOREPARAMS";
    }
    else if (usr_exmpl->getFlagReg()){
        res.code_response = "462";
        res.str_response = "ERR_ALREADYREGISTRED";
    }
    else if (count_arg >= 4)
    {
        tmp_usr.push_back(tmp_arg[0]);
        tmp_usr.push_back(tmp_arg[1]);
        tmp_usr.push_back(tmp_arg[2]);

        for (int i = 3; i < count_arg; i++){
            tmp_realname = tmp_realname + tmp_arg[i];
            tmp_realname += " ";
        }
        tmp_realname.erase(tmp_realname.begin());
        tmp_realname.erase(tmp_realname.end());
        tmp_usr.push_back(tmp_realname);
        if (usr_exmpl->getUserUser() == "Undefined")
            usr_exmpl->setUserUser(tmp_usr);
        if (usr_exmpl->getNickUser() != "Undefined") {
            usr_exmpl->setFlagReg();
            res.code_response = "001";
            res.str_response = "RPL_WELCOME";
            usr_exmpl->sendMTD();
        }
    }
    return (res);
}


response_server Serv::privmsg(Request comm_exmpl, User *usr_exmpl) {
    response_server res;
    std::vector<std::string> tmp_arg = comm_exmpl.get_vect_arg();
    std::string reciever;
    User *usr_reciever;
    Channel* tmp_channel;
    if (tmp_arg.size() < 2) {
        if (tmp_arg.size() == 0) {
            res.code_response = "411";
            res.str_response = "ERR_NORECIPIENT";
        } else if (tmp_arg.size() == 1) {
            res.code_response = "412";
            res.str_response = "ERR_NOTEXTTOSEND";
        }
        return (res);
    }
    reciever = tmp_arg[0];
    if((reciever.find('#') != std::string::npos)){
		tmp_channel = getChannel(reciever);
		if (tmp_channel){
			tmp_channel->sendPrivChannel(tmp_arg, usr_exmpl->getNickUser());
		}
    }
    else{
        usr_reciever = getUser(reciever);
        if (!usr_reciever){
            res.code_response = "401";
            res.str_response = "ERR_NOSUCHNICK";
        }
        else{
            usr_reciever->sendPrivMSG(comm_exmpl, usr_exmpl->getNickUser());
            res.code_response = "303";
            res.str_response = reciever;
        }
    }
    return (res);
}

response_server Serv::notice(Request comm_exmpl, User *usr_exmpl) {
    response_server res;
    std::vector<std::string> tmp_arg = comm_exmpl.get_vect_arg();
    std::string reciever;
    User *usr_reciever;
    Channel* tmp_channel;
    if (tmp_arg.size() < 2) {
        if (tmp_arg.size() == 0) {
            res.code_response = "";
        } else if (tmp_arg.size() == 1) {
            res.code_response = "";
        }
        return (res);
    }
    reciever = tmp_arg[0];
    if((reciever.find('#') != std::string::npos)){
		tmp_channel = getChannel(reciever);
		if (tmp_channel){
			tmp_channel->sendPrivChannel(tmp_arg, usr_exmpl->getNickUser());
		}
    }
    else{
        usr_reciever = getUser(reciever);
        if (usr_reciever){
            usr_reciever->sendPrivMSG(comm_exmpl, usr_exmpl->getNickUser());
        }
    }
    return (res);
}

response_server Serv::join(int fd_client, Request comm_exmpl, User *usr_exmpl) {
    response_server res;
    std::vector<std::string> tmp_arg = comm_exmpl.get_vect_arg();
    Channel* tmp_channel;
    User* tmp_user;
    if (tmp_arg.size() == 0){
        res.code_response = "461";
        res.str_response = "ERR_NEEDMOREPARAMS";
    }
    else if (tmp_arg.size() == 1 && tmp_arg[0][0] != '#'){
        sendNoUser(fd_client, "403 " + usr_exmpl->getNickUser() + " " + toLowerString(tmp_arg[0]), "ERR_NOSUCHCHANNEL");
    }
    else if(tmp_arg.size() == 1) {
        tmp_channel = getChannel(toLowerString(tmp_arg[0]));
        if (tmp_channel){
        	tmp_user = tmp_channel->getUserChannel(usr_exmpl->getNickUser());
        	if (!tmp_user)
			{
				tmp_channel->addUserChannel(usr_exmpl);
				tmp_channel->sendJoinAll(usr_exmpl->getNickUser());
                tmp_channel->sendReplaySenderJoin(usr_exmpl->getNickUser());
			}
        }
        else{
            sendNoUser(fd_client, "403 " + usr_exmpl->getNickUser() + " " + toLowerString(tmp_arg[0]), "ERR_NOSUCHCHANNEL");
            channels.push_back(new Channel(toLowerString(tmp_arg[0]), getVectUser()));
            tmp_channel = getChannel(toLowerString(tmp_arg[0]));
            if (tmp_channel) {
                tmp_channel->addUserChannel(usr_exmpl);
                tmp_channel->sendReplaySenderJoin(usr_exmpl->getNickUser());
                tmp_channel->sendJoinAll(usr_exmpl->getNickUser());
            }
        }
    }
    else{
        res.code_response = "405";
        res.str_response = "ERR_TOOMANYCHANNELS";
    }
    return (res);
}


response_server Serv::who(Request comm_exmpl) {
	response_server res;
	std::vector<std::string> tmp_arg = comm_exmpl.get_vect_arg();
	Channel* tmp_channel;
	if (tmp_arg.size() >= 1 && tmp_arg[0].size() > 0){
		if (tmp_arg[0][0] == '#'){
			tmp_channel = getChannel(tmp_arg[0]);
			if(tmp_channel){
				res.str_response = tmp_channel->getWhoChannel();
				res.code_response = "315";
			}
		}
	}
	return (res);
}

response_server Serv::pingClient(int fd_client, Request comm_exmpl) {
    response_server res;
    std::vector<std::string> tmp_arg = comm_exmpl.get_vect_arg();
    std::string str_replay;
    str_replay = "PONG " + tmp_arg[0] + "\r\n";
    write(fd_client, str_replay.c_str(), str_replay.length());
    res.code_response = "";
    return(res);
}

response_server Serv::pongClient(Request comm_exmpl, User *usr_exmpl, int index_fd) {
    response_server res;
    std::vector<std::string> tmp_arg = comm_exmpl.get_vect_arg();
    std::time_t result;
    if (tmp_arg.size() == 1){
        if (tmp_arg[0] == "1648063017"){
            result = std::time(NULL);
            if(usr_exmpl)
                usr_exmpl->setTimeStamp(result);
            arr_timestamp[index_fd] = std::time(NULL);
        }
    }
    res.code_response = "";
    return (res);
}

response_server Serv::oper(Request comm_exmpl, User *usr_exmpl) {
    response_server res;
    std::vector<std::string> tmp_arg = comm_exmpl.get_vect_arg();
    if (tmp_arg.size() < 2){
        res.code_response = "461";
        res.str_response = "ERR_NEEDMOREPARAMS";
    }
    else if(_oper_pass != tmp_arg[1]){
        res.code_response = "464";
        res.str_response = "ERR_PASSWDMISMATCH";
    }
    else if (_oper_user == tmp_arg[0]){
        res.code_response = "381";
        res.str_response = "RPL_YOUREOPER";
        usr_exmpl->setFlagOper();
    }
    return (res);
}

response_server Serv::quit(Request comm_exmpl, User *usr_exmpl, int index_fd) {
    User *tmp_user_for_del;
    int tmp_fd;
    std::vector<std::string> tmp_arg = comm_exmpl.get_vect_arg();
    response_server res;
    tmp_fd = usr_exmpl->getFdUser();
    tmp_user_for_del = *(getUserIter(tmp_fd));
    if (index_fd != 0)
        fd_list[index_fd].fd = -1;
    if (getUserIter(tmp_fd) != _users.end()) {
        sendQuitUser(usr_exmpl->getNickUser(), tmp_arg);
        clearChannel(usr_exmpl->getNickUser());
        _users.erase(getUserIter(tmp_fd));
        delete tmp_user_for_del;
    }
    if (tmp_fd != -1)
        close(tmp_fd);
    res.code_response = "";
    return (res);
}

response_server Serv::kick(Request comm_exmpl, User *usr_exmpl) {
    std::vector<std::string> tmp_arg = comm_exmpl.get_vect_arg();
    response_server res;
    Channel *tmp_channel;
    if (tmp_arg.size() < 2){
        res.code_response = "461";
        res.str_response = "ERR_NEEDMOREPARAMS";
        return (res);
    }
    tmp_channel = getChannel(tmp_arg[0]);
    if (!tmp_channel){
        res.code_response = "403";
        res.str_response = "ERR_NOSUCHCHANNEL";
    }
    else if (usr_exmpl->getNickUser() != tmp_channel->getNickOperChannel()) {
        res.code_response = "403";
        res.str_response = "ERR_NOSUCHCHANNEL";
    }
    else{
        tmp_channel->eraseUserFromChannel(tmp_arg[1]);
    }
    return (res);
}

response_server Serv::kill(Request comm_exmpl, User *usr_exmpl) {
    std::vector<std::string> tmp_arg = comm_exmpl.get_vect_arg();
    response_server res;
    User *drop_user;
    int fd_drop_user;
    int index_fd = 0;
    if (usr_exmpl->getFlagOper() == 0){
        res.code_response = "481";
        res.str_response = "ERR_NOPRIVILEGES";
    }
    else if (tmp_arg.size() < 1){
        res.code_response = "461";
        res.str_response = "ERR_NEEDMOREPARAMS";
    }
    else if(!(getUser(tmp_arg[0]))){
        res.code_response = "401";
        res.str_response = "ERR_NOSUCHNICK";
    }
    else{
        drop_user = getUser(tmp_arg[0]);
        fd_drop_user = drop_user->getFdUser();
        tmp_arg.erase(tmp_arg.begin());
        clearChannel(usr_exmpl->getNickUser());
        _users.erase(getUserIter(fd_drop_user));
        delete drop_user;
        close(fd_drop_user);
        index_fd = getIndexFd(fd_drop_user);
        if (index_fd)
            fd_list[index_fd].fd = -1;
    }
    return (res);
}

response_server Serv::list(User *usr_exmpl) {
    std::vector<Channel*>::iterator iter_begin = channels.begin();
    std::vector<Channel*>::iterator iter_end = channels.end();
    std::string mes_321;
    std::string mes_322;
    std::string mes_323;
    response_server res;
    mes_321 = ":IRC 321 " + usr_exmpl->getNickUser() + " Channel :Users Name\n\r";
    mes_323 = ":IRC 323 " + usr_exmpl->getNickUser() + " :End of /LIST\n\r";
    write(usr_exmpl->getFdUser(), mes_321.c_str(), mes_321.length());
    for (; iter_begin != iter_end; iter_begin++){
        mes_322 = ":IRC 322 " + usr_exmpl->getNickUser() + " " + (*iter_begin)->getNameChannel() + " " + (*iter_begin)->getNumberUsers() + "\n\r";
        write(usr_exmpl->getFdUser(), mes_322.c_str(), mes_322.length());
    }
    write(usr_exmpl->getFdUser(), mes_323.c_str(), mes_323.length());
    res.code_response = "";
    return (res);
}

User *Serv::getUser(int fd) {
    std::vector<User*>::iterator iter = _users.begin();
    std::vector<User*>::iterator iter_end = _users.end();
    for (;iter != iter_end; iter++){
        if (fd == (*iter)->getFdUser())
            return (*iter);
    }
    return (NULL);
}

User *Serv::getUser(std::string nick) {
    std::vector<User*>::iterator iter = _users.begin();
    std::vector<User*>::iterator iter_end = _users.end();

    for (;iter != iter_end; iter++){
        if (nick == (*iter)->getNickUser())
            return (*iter);
    }
    return (NULL);
}

std::vector<User *>::iterator Serv::getUserIter(int fd) {
    std::vector<User*>::iterator iter = _users.begin();
    std::vector<User*>::iterator iter_end = _users.end();

    for (;iter != iter_end; iter++){
        if (fd == (*iter)->getFdUser())
            return (iter);
    }
    return (iter);
}

int Serv::checkNick(std::string nick) {
    size_t nick_size = nick.size();
    for (size_t i = 0; i < nick_size; i++) {
        if ((nick[i] >= 'a' && nick[i] <= 'z') || (nick[i] >= 'A' && nick[i] <= 'Z')
            || (nick[i] >= '0' && nick[i] <= '9'))
            continue;
        else
            return (0);
    }
    if (getUser(nick))
        return (1);
    return (2);
}

int Serv::getCountCommand(char *buf) {
    int count = 0;
    std::string str(buf);
    for (size_t i = 0; i < str.length(); i++){
        if (str[i] == '\n')
            count++;
    }
    return (count);
}

std::string Serv::getTmpBuf(int count, char *buf) {
    std::string res(buf);
    std::string tmp_res = "";
    size_t pos;
    while (res.find("\r\n") != std::string::npos)
        res.replace(res.find("\r\n"), 2, "\n");
    if (count == 0){
        pos = res.find('\n');
        res.erase(pos);
        return (res);
    }
    else if (count == 1){
        pos = res.find('\n');
        tmp_res = res.substr(pos + 1);
        pos = tmp_res.find('\n');
        tmp_res.erase(pos);
        return (tmp_res);
    }
    else if (count == 2) {
        pos = res.find('\n');
        tmp_res = res.substr(pos + 1);
        pos = tmp_res.find('\n');
        res.clear();
        res = tmp_res.substr(pos + 1);
        return (res);
    }
    return ("o_O");
}

void Serv::sendNoUser(int fd, std::string code, std::string text) {
        std::string response_serv = ":IRC " + code + " " + " : " + text + "\r\n";
        write(fd, response_serv.c_str(), response_serv.length());
}

std::vector<User*> *Serv::getVectUser(){
    return (&_users);
}

Channel* Serv::getChannel(std::string channel_name) {
    std::vector<Channel*>::iterator it_begin = channels.begin();
    std::vector<Channel*>::iterator it_end = channels.end();
    for (; it_begin != it_end; it_begin++){
        if ((*it_begin)->getNameChannel() == channel_name)
            return (*it_begin);
    }
    return (NULL);
}

void Serv::clearChannel(std::string name_user) {
    std::vector<Channel*>::iterator it_chan_begin;
    std::vector<Channel*>::iterator it_chan_end;
    if (channels.size() > 0){
        it_chan_begin = channels.begin();
        it_chan_end = channels.end();
        for(;it_chan_begin != it_chan_end; it_chan_begin++)
            (*it_chan_begin)->eraseUserFromChannel(name_user);
    }
}

void Serv::sendQuitUser(std::string name_user, std::vector<std::string> tmp_arg_1) {
    response_server res;
    std::vector<User*>::iterator it_begin;
    std::vector<User*>::iterator it_end;
    std::string message;
    std::string replay;
    int index_fd;
    if (_users.size() > 0){
        it_begin = _users.begin();
        it_end = _users.end();
        if (tmp_arg_1.size() > 0)
            message = getMessageServ(tmp_arg_1);
        else
            message = "No reason quit";
        replay = ":" + name_user + "!Adium@127.0.0.1 QUIT :" + message + "\r\n";
        for (; it_begin != it_end; it_begin++){
            index_fd = getIndexFd((*it_begin)->getFdUser());
            if (index_fd < 1)
                break ;
            if (fd_list[index_fd].fd != -1 && index_fd > 0)
                write((*it_begin)->getFdUser(), replay.c_str(), replay.length());
        }
    }
}

std::string Serv::getMessageServ(std::vector<std::string> vect_arg) {
    std::string res = "";
    if (vect_arg.size() == 0)
        return (res);
    if (vect_arg.size() == 1){
        res = vect_arg[0];
        if (res[0] == ':') {
            res.erase(0);
        }
    }
    else if(vect_arg.size() > 1){
        if (vect_arg[0][0] != ':')
            res = vect_arg[0];
        else {
            for (size_t i = 0; i < vect_arg.size(); i++)
                res += vect_arg[i] + " ";
            res.erase(res.end() - 1);
        }
    }
    return (res);
}

int Serv::getIndexFd(int fd) {
    for (int i = 0; i < MAX_USERS; i++){
        if (fd == fd_list[i].fd)
            return (i);
    }
    return (0);
}

std::string Serv::toLowerString(std::string strUp) {
    for (size_t i = 0; i < strUp.size(); i++)
      strUp[i] = ::tolower(strUp[i]);
    return (strUp);
}
