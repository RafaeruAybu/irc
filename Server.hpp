#ifndef SERVER_HPP
#define SERVER_HPP
//cpp headers
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "Client.hpp"
#include "Channel.hpp"
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
#define MAX_USERS 1024      //user nums in 
#define POLL_TIMEOUT 3000   //poll timeout time
#define BUFF_SIZE 512       //fd read buff size

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
    //todo implement, maybee more!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!yes
    void add_user();
    void delete_user();
    void add_channel();
    void delete_channel();
    void add_user_to_channel();
    void delete_user_from_channel();
    void add_user_to_operators();
    void delete_user_from_operators();
    void kill();    //it is kiLL
    void kick();    //it is kiCK
    void ban();     //тупа бан
    //params
    int listen_socket;                  //listening socket
    int password;                       //servers password (PASS 12345678)
    int num;                            //user num including listener
    struct pollfd fd_list[MAX_USERS];   //fd list to poll
    std::vector<Client> users;          //userlist
    std::vector<Channel> channels;      //channels
};

#endif //SERVER_HPP
