#ifndef SERVER_HPP
#define SERVER_HPP
//cpp headers
#include <iostream>
#include <string>
//c headers
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//defines
#define MAX_USERS 1024
#define POLL_TIMEOUT 3000 //poll timeout time
#define BUFF_SIZE 512

class Serv
{
public:
    Serv(std::string port, std::string password);
    void get_into_loop();
    ~Serv();

private:
    //default holy
    Serv();
    Serv(const Serv &other);
    Serv& operator= (const Serv& other);

    //foos
    int get_listen_sock(int port);
    //params
    int listen_socket;  //listening socket
    int password;       //servers password (PASS 12345678)
    struct pollfd fd_list[MAX_USERS]; //fd list to poll
    int num;              //user num including listener

};

#endif //SERVER_HPP
