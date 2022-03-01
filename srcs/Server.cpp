#include "../includes/Server.hpp"
#include "../includes/Request.hpp"

//init server
Serv::Serv(char *port, char *password)
{
    exit_server = false;
    _str_password = password; //new 20.02
    this->password = atoi(password);
    listen_socket = get_listen_sock(atoi(port)); //get listen sock
    num = sizeof(fd_list) / sizeof(fd_list[0]); //num of fds
    int i = 0;
    for (; i < num; i++) //init fd_list
    {
        fd_list[i].fd = -1;
        fd_list[i].events = 0; //set of events
        fd_list[i].revents = 0; //r event???
    }
    i = 0;
    for (; i < num; i++) // add read-only events
    {
        if (fd_list[i].fd == -1)
        {
            fd_list[i].fd = listen_socket;
            fd_list[i].events = POLLIN;
            break;
        }
    }
    //init bufs
    i = 0;
    for (; i < MAX_USERS; i++) //user
    {
        for (int j = 0; j < BUFF_SIZE; j++) //char
        {
            bufs[i][j] = 0;
        }
    }
}

//delete server
Serv::~Serv()
{

}

//get listening socket
int Serv::get_listen_sock(int port)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0); // get new socket of type sock_stream ipv4
    if (sock < 0) //check if good todo can be done using macros
        throw "failed getting sock...";
    int opt = 1;
    if (fcntl(sock, F_SETFL, O_NONBLOCK))
        throw "Could not set non-blocking socket...";
    setsockopt(sock, SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt)); //todo explain
    struct sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = htonl(INADDR_ANY); //any type address
    local.sin_port = htons(port); //port number to listen

    //binding port
    if (bind(sock,(struct sockaddr *)&local, sizeof(local)) < 0)
        throw "bind failed...";
    
    //prepare sock to listen
    if (listen(sock, 5) < 0)
        throw "listen failed";
    
    return sock;
}

//todo maybe implement close server
void Serv::do_poll_timeout()
{
//    std::cout << "poll timeout..." << std::endl; ////Чтоб не спамил)
    // if (fcntl(0, F_SETFL, O_NONBLOCK)) //make new sock is non-blocking
    //             throw "Could not set non-blocking socket...";
    // char server_command_str[50];
    // ssize_t size = read(0, server_command_str, sizeof(server_command_str) - 1);
    // if (size < 0)
    // {
    //     if (errno != EWOULDBLOCK)
    //                 std::cout << "read failed..." << std::endl;
    //     continue;
    // }
    // else
    //     server_command_str[size] = '\0';
    // if (strstr(server_command_str, "stop"))
    //     exit_server = true;
}

void Serv::do_poll_fail()
{
    std::cout << "poll fail..." << std::endl;
}

int Serv::get_new_connection()
{
    //provide a connectinon acceptance here
    struct sockaddr_in client;
    socklen_t len = sizeof(client);
    int new_sock = accept(listen_socket,(struct sockaddr *)&client,&len);
    if (new_sock < 0)
    {
        if (errno != EWOULDBLOCK)
            std::cout << "accept fail..." << std::endl;
        return -1; //stop if failed to accept
    }
    if (fcntl(new_sock, F_SETFL, O_NONBLOCK)) //make new sock is non-blocking
        throw "Could not set non-blocking socket...";
    int i = 0;
    for (; i < num; i++)
    {
        if (fd_list[i].fd == -1)
            break;
    }
    if (i < num)
    {
        fd_list[i].fd = new_sock;
        fd_list[i].events = POLLIN;
    }
    else
        close(new_sock);
    std::cout
        << "get a new connection[" << inet_ntoa(client.sin_addr)
        << ":" << ntohs(client.sin_port) << "]" << std::endl;
    return 0;
}

void Serv::do_poll_default()
{
    //if it is a listener fd, call an accept to accept new connection
    //if it is a client fd, read the data
    int i = 0;
    for (; i < num; i++) //for every fd
    {
        if (fd_list[i].fd == -1) //if fd sock is free(empty)
            continue;
        if (fd_list[i].fd == listen_socket && (fd_list[i].revents & POLLIN)) //if socket is listen_sock and pollin event
        {
            if (get_new_connection() != 0)
                break ;
            else
                continue ;
        }

        //todo refactor code like: process_connection();
        // For all normall fds
        if (fd_list[i].revents & POLLIN)
        {
            char buf[BUFF_SIZE];
            ssize_t s = read(fd_list[i].fd, buf, sizeof(buf)-1);
            if (s < 0)
            {
                if (errno != EWOULDBLOCK)
                    std::cout << "read failed..." << std::endl;
                break;
            }
            else if (s == 0)
            {
                std::cout << "client quit..." << std::endl;
                close(fd_list[i].fd);
                fd_list[i].fd = -1;
                break;   //todo test
            }
            else
                buf[s] = 0; //null terminate
            //buf is (char [512]) array, so i do cast to just (char *)
            process(fd_list[i].fd, (char *)&buf);
        }
    }
}

void Serv::get_into_loop()
{
    while (!exit_server) //todo end_server
    {
        switch(poll(fd_list, num, POLL_TIMEOUT)) // 3000 is a timeout time
        {
            case 0: //timeout
                do_poll_timeout();
                continue ;
            case -1: //fails
                do_poll_fail();
                continue ;
            default: //if poll worked as expected
                do_poll_default();
                break ;
        }
    }
    return ;
}

//Welcom to processor m8. Here using magic, buf itself, gets its meaning.
//Here Zh3nuke AND S4ny4 are going to do their jobs.
//Amine.
//fd - clients fd, buf - "\0" terminated commands from client.
//get your line
void Serv::process(int fd, char *buf)
{
    //
    response_server my_response;
//    Request command_exmpl(buf);
    Request *command_exmpl;
    User* usr_exmpl = getUser(fd);
    int count_command;
    std::string tmp_buf;

//    User* tmp_user = getUser(fd); //Ищем в _users совпадение по int fd, костыльно конечно с индексом, но с итератором не заработало
    std::string response_serv = "";

    std::cout << "User[" << fd << "]: " << buf << std::endl;
    count_command = getCountCommand(buf);

    //todo delete and do it right
//    for (int i = 0; i < count_command; i++) {
//        tmp_buf = getTmpBuf(i, buf);
        command_exmpl = new Request(buf); //todo: Поменять на tmp_buf, когда разберусь с несколькими \r\n в строке
        if (command_exmpl->get_comm() == "PASS")
            my_response = pass(fd, *command_exmpl, usr_exmpl);
        else if (command_exmpl->get_comm() == "USER") {
            my_response = user(fd, *command_exmpl, usr_exmpl);
        } else if (command_exmpl->get_comm() == "NICK")
            my_response = nick(fd, *command_exmpl, usr_exmpl);
        if (usr_exmpl) { //комманды кроме PASS, NICK, USER не обрабатываются, если User не был добавлен через PASS
            if (command_exmpl->get_comm() == "OPER") {}
            else if (command_exmpl->get_comm() == "QUIT") {}
            else if (command_exmpl->get_comm() == "PRIVMSG") {}
            else if (command_exmpl->get_comm() == "NOTICE") {}
            else if (command_exmpl->get_comm() == "JOIN") {}
            else if (command_exmpl->get_comm() == "KICK") {}
            else if (command_exmpl->get_comm() == "MODE") {}

        }
        if (my_response.str_response.length() != 0) //Если есть числовые ответы - формируем строку для вывода в fd
            response_serv = my_response.code_response + " : " + my_response.str_response + "\r\n";
//    std::cout << response_serv << "\n";
        write(fd, response_serv.c_str(), response_serv.length()); // Отправка ответа в сокет
//    }

    //Ответ должен иметь вид ":IRCat 433 dduck dduck :Nickname is already in use"

}


////// Command_method

response_server Serv::pass(int fd_client, Request comm_exmpl, User *usr_exmpl) {
	response_server res;
    std::vector<std::string> tmp_arg = comm_exmpl.get_vect_arg();

    if ((tmp_arg[0].size() != 0) && (tmp_arg[0][0] == ':'))
        tmp_arg[0].erase(tmp_arg[0].begin());

    if (usr_exmpl){ // уже есть в vector<Users>
        res.code_response = "462";
        res.str_response = "ERR_ALREADYREGISTRED";
    }
    else if (tmp_arg.size() == 0){ // введен только PASS
        res.code_response = "461";
        res.str_response = "ERR_NEEDMOREPARAMS";
    }
    else if (tmp_arg[0] == _str_password || _str_password.length() == 0) { //valid pass
        _users.push_back(new User(fd_client)); // Написан новый конструктор для fd
        res.code_response = "OK PASS"; //todo: for test, need deleted
        res.str_response = "OK PASS";
    }
	return (res);
}

response_server Serv::nick(int fd_client, Request comm_exmpl, User *usr_exmpl) {
    response_server res;
    std::vector<std::string> tmp_arg = comm_exmpl.get_vect_arg();
    int check_res = 0;

    if (tmp_arg.size() != 0)
        check_res = checkNick(tmp_arg[0]);
//    check_res = checkNick();
    if (tmp_arg.size() == 0){ // введен только NICK
        res.code_response = "431";
        res.str_response = "ERR_NONICKNAMEGIVEN";
    }
    else if (!check_res){
        res.code_response = "432";
        res.str_response = "ERR_ERRONEUSNICKNAME";
    }
    else if (check_res == 1){
        res.code_response = "433";
        res.str_response = "ERR_NICKNAMEINUSE";
    }
    else{
        usr_exmpl->setNick(tmp_arg[0]);
        if (usr_exmpl->getUserUser() != "Undefined")
            usr_exmpl->setFlagReg();

    }
    return (res);
    //ERR_NONICKNAMEGIVEN не указан
    //ERR_ERRONEUSNICKNAME не валид
    //ERR_NICKNAMEINUSE уже используется
    //ERR_NICKCOLLISION уже используется, ответ сервера другому серверу
}

response_server Serv::user(int fd_client, Request comm_exmpl, User *usr_exmpl) {
    response_server res;
    std::vector<std::string> tmp_arg = comm_exmpl.get_vect_arg();

    std::cout << "USER\n";

    int count_arg = tmp_arg.size();
    std::vector<std::string> tmp_usr;
//    std::string tmp_usrname = "";
//    std::string tmp_hostname= "";
//    std::string tmp_servname = "";
    std::string tmp_realname = "";



    if (((count_arg >= 4) && (tmp_arg[3][0] != ':')) || (count_arg < 4)){
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

//        tmp_usrname = tmp_arg[0];
//        tmp_hostname = tmp_arg[1];
//        tmp_servname = tmp_arg[2];
        for (int i = 3; i < count_arg; i++){
            tmp_realname = tmp_realname + tmp_arg[i];
            tmp_realname += " ";
        }
        tmp_realname.erase(tmp_realname.begin());
        tmp_realname.erase(tmp_realname.end()); //del last " "
        std::cout << "tmp_realname: " << tmp_realname << "\n";
        tmp_usr.push_back(tmp_realname);
        usr_exmpl->setUserUser(tmp_usr);
        if (usr_exmpl->getNickUser() != "Undefined") {
            usr_exmpl->setFlagReg();
            res.code_response = "001 rafa ";
            res.str_response = "Welcome to server!!!\r\n";
        }
    }
    //Ответ должен иметь вид ":IRCat 433 dduck dduck :Nickname is already in use"
    return (res);

    //USER guest tolmoon tolsun :Ronnie Reagan kek puk
    //:testnick USER guest tolmoon tolsun :Ronnie Reagan
}

////command utils
User *Serv::getUser(int fd) {
    std::vector<User*>::iterator iter = _users.begin();
    std::vector<User*>::iterator iter_end = _users.end();
    int i = 0;

    for (;iter != iter_end; iter++){
        std::cout << "getUser fd=" << fd << ", get_fd=" << (*iter)->getFdUser() << ", i=" << i << "\n";
        if (fd == (*iter)->getFdUser()) {
            std::cout << "Нашел ! fd = " << (*iter)->getFdUser() << "\n";
            return (*iter);
        }
        i++;
    }
    return (NULL);
}

User *Serv::getUser(std::string nick) {
    std::vector<User*>::iterator iter = _users.begin();
    std::vector<User*>::iterator iter_end = _users.end();
    int i = 0;

    for (;iter != iter_end; iter++){
        std::cout << "getUser nick=" << nick << ", get_nick=" << (*iter)->getNickUser() << ", i=" << i << "\n";
        if (nick == (*iter)->getNickUser()) {
            return (*iter);
        }
        i++;
    }
    return (NULL);
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
    std::istringstream iss(buf);
    std::vector<std::string> results((std::istream_iterator<std::string>(iss)),
                                     std::istream_iterator<std::string>());

    size_t size_vect_buf = results.size();
    int count = 0;

    for (size_t i = 0; i < size_vect_buf; i++){
        if (results[i].find("\r\n") != std::string::npos)
            count++;
    }
    std::cout << "size_vect_buf: " << size_vect_buf << "\n";

    std::cout << "getCountCommand: " << count << "\n";
    return (count);
}

std::string Serv::getTmpBuf(int count, char *buf) {
    std::istringstream iss(buf);
    std::vector<std::string> results((std::istream_iterator<std::string>(iss)),
                                     std::istream_iterator<std::string>());

    std::string res = "";

    if (count < 2) {
        std::cout << "res count=1: " << buf << "\n";
        return (buf);
    }
    else if (count == 2){
        for (int i = 0; i < results.size(); i++) {
            if (results[i].find("\r\n") != std::string::npos) {
                for (int j = i + 1; j < results.size() - 1; j++){
                    res += results[j] + " ";
                    if (results[j].find("\r\n") != std::string::npos)
                        break ;
                }
                res.erase(res.end());
                std::cout << "res count=2: " << res << "\n";
                return (res);
            }
        }
    }
    else{
        for (int i = 0; i < results.size(); i++) {
            if (results[i].find("\r\n") != std::string::npos) {
                for (int j = i + 1; j < results.size() - 1; j++){
                    if (results[j].find("\r\n") != std::string::npos){
                        for (int k = j + 1; k < results.size() - 2; k++){
                            res += results[k] + " ";
                            if (results[k].find("\r\n") != std::string::npos)
                                break ;
                        }
                        res.erase(res.end());
                        std::cout << "res count=3: " << res << "\n";
                        return (res);
                    }
                }
            }
        }
    }
    return ("o_O");
}



