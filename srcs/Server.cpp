#include "../includes/Server.hpp"
#include "../includes/Command.hpp"


//init server
Serv::Serv(char *port, char *password)
{
    exit_server = false;
    str_password = password; //new 20.02
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
    std::cout << "poll timeout..." << std::endl;
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
    Command command_exmpl(buf);


    //

    std::cout << "Client[" << fd << "]: " << buf << std::endl;
    //todo delete and do it right

    if (command_exmpl.get_comm() == "PASS")
        add_user();
    else if (command_exmpl.get_comm() == "NICK")
        add_user_nick();
    else if (command_exmpl.get_comm() == "USER"){
        add_user_user();
        write(fd, "001 rafa :Welcome to server!!!\r\n", strlen("001 rafa :Welcome to server!!!\r\n"));
    }
//    if (strstr(buf, "USER"))
//    {
//        write(fd, "001 rafa :Welcome to server!!!\r\n", strlen("001 rafa :Welcome to server!!!\r\n"));
//    }
}




////// Command_method

void Serv::add_user() {

}

void Serv::add_user_nick() {

}

void Serv::add_user_user() {

}

