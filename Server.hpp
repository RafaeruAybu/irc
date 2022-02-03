#ifndef SERVER_HPP
#define SERVER_HPP
//cpp headers
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "Client.hpp"
// #include <basic_string.h>
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
    Serv();
    Serv(const Serv &other);
    Serv& operator= (const Serv& other);

    //foos
    int get_listen_sock(int port);
    void process(int fd, char *buf);
    //params
    int listen_socket;  //listening socket
    int password;       //servers password (PASS 12345678)
    int num;              //user num including listener
    struct pollfd fd_list[MAX_USERS]; //fd list to poll
    std::vector<Client> users; //userlist

};

#endif //SERVER_HPP
