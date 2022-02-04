#include "../includes/Server.hpp"

//init server
Serv::Serv(std::string port, std::string password)
{
    this->password = atoi(password.c_str());
    listen_socket = get_listen_sock(atoi(port.c_str())); //get listen sock
    num = sizeof(fd_list) / sizeof(fd_list[0]); //num of fds
    int i = 0;
    for (; i < num; i++) //init
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
    setsockopt(sock, SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt)); //todo explain
    if (fcntl(sock, F_SETFL, O_NONBLOCK))
        throw "Could not set non-blocking socket...";
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

void Serv::get_into_loop()
{
    while (1)
    {
        switch( poll(fd_list, num, POLL_TIMEOUT)) // 3000 is a timeout time
        {
            case 0:
                // std::cout << "poll timeout..." << std::endl;
                continue;
            case -1:
                std::cout << "poll fail..." << std::endl;
                continue;
            default: //if poll worked as expected
            {
                //if it is a listener fd, call an accept to accept new connection
                //if it is a client fd, read the data
                int i = 0;
                for (; i < num; i++) //for every fd
                {
                    if (fd_list[i].fd == -1)
                        continue;
                    if (fd_list[i].fd == listen_socket && (fd_list[i].revents & POLLIN))
                    {
                        //provide a connectinon acceptance here
                        struct sockaddr_in client;
                        socklen_t len = sizeof(client);
                        int new_sock;
                        if (fcntl(new_sock, F_SETFL, O_NONBLOCK))
                            throw "Could not set non-blocking socket...";
                        new_sock = accept(listen_socket,(struct sockaddr *)&client,&len);
                        if (new_sock < 0)
                        {
                            std::cout << "accept fail..." << std::endl;
                            continue;
                        }
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
                        continue;
                    }
                    // For all normall fds
                    if (fd_list[i].revents & POLLIN)
                    {
                        char buf[BUFF_SIZE];
                        ssize_t s = read(fd_list[i].fd, buf, sizeof(buf)-1);
                        if (s < 0)
                            std::cout << "read failed..." << std::endl;
                        else if (s == 0)
                        {
                            std::cout << "client quit..." << std::endl;
                            close(fd_list[i].fd);
                            fd_list[i].fd = -1;
                        }
                        else
                            buf[s] = 0; //null terminate
                        //buf is (char [512]) array, so i do cast to just (char *)
                        process(fd_list[i].fd, (char *)&buf);
                    }
                }
            }
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
    (void) fd; //unvoid it and use
    (void) buf; //unvoid it and use
    std::cout << "Client[" << fd << "]: " << buf << std::endl;
    //todo delete and do it right
    if (strstr(buf, "USER"))
    {
        write(fd, "001 rafa :Welcome to server!!!\r\n", strlen("001 rafa :Welcome to server!!!\r\n"));
    }
}
