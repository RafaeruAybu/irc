#ifndef SERVER_HPP
# define SERVER_HPP
//cpp headers
#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <map>
#include <map>
#include "User.hpp"
#include "Channel.hpp"
#include "Request.hpp"
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
#include <fcntl.h>
#include <errno.h>

//defines
#define MAX_USERS 1024      //user nums in 
#define POLL_TIMEOUT 3000   //poll timeout time
#define BUFF_SIZE 512       //fd read buff size

typedef struct response_server{
	std::string code_response;
	std::string str_response;
} response_server;

class Serv
{
public:
    Serv(char *port, char *password);
    void get_into_loop();
    ~Serv();



private:
    Serv();
    Serv(const Serv &other);
    Serv& operator= (const Serv& other);

    //foos
    int get_listen_sock(int port);      
    void process(int fd, char *buf);
    void do_poll_timeout();
    void do_poll_fail();
    void do_poll_default();
    int get_new_connection();

    //todo foos
    response_server pass(int fd_client, Request comm_exmpl, User *usr_exmpl);
    response_server nick(int fd_client, Request comm_exmpl, User *usr_exmpl);
    response_server user(int fd_client, Request comm_exmpl, User *usr_exmpl);
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
    char bufs[MAX_USERS][BUFF_SIZE];    //4 Mb size in stack
    bool exit_server;
    int listen_socket;                  //listening socket

    std::string _str_password; //new 20.02
    int password;                       //servers password (PASS 12345678)
    int num;                            //user num including listener
    struct pollfd fd_list[MAX_USERS];   //fd list to poll
    std::vector<User*> _users;          //userlist
    std::vector<Channel> channels;      //channels

    std::string _oper_user;
    std::string _oper_pass;

    ////command utils
    User *getUser(int fd);
    std::vector<User*>::iterator getUserIter(int fd);
    User *getUser(std::string nick);
    int checkNick(std::string nick);
    int getCountCommand(char *buf);
    std::string getTmpBuf(int count, char *buf);

};

#endif //SERVER_HPP
