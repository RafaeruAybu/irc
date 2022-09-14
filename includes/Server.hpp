#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <map>
#include <map>
#include "User.hpp"
#include "Channel.hpp"
#include "Request.hpp"

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
#include <algorithm>

#define MAX_USERS 1024      //user nums in 
#define POLL_TIMEOUT 3000   //poll timeout time
#define BUFF_SIZE 512       //fd read buff size
#define TIMEOUT 10          //timeout PING seconds

typedef struct response_server {
	std::string code_response;
	std::string str_response;
} response_server;

class Serv {
public:
    Serv(char *port, char *password);
    void get_into_loop();
    ~Serv();

private:
    Serv();
    Serv(const Serv &other);
    Serv& operator= (const Serv& other);

    int get_listen_sock(int port);      
    void process(int fd, char *buf, int index_fd);
    void do_poll_timeout();
    void do_poll_fail();
    void do_poll_default();
    int get_new_connection();

    response_server pass(int fd_client, Request comm_exmpl, User *usr_exmpl);
    response_server nick(int fd_client, Request comm_exmpl, User *usr_exmpl);
    response_server user(int fd_client, Request comm_exmpl, User *usr_exmpl);
    response_server privmsg(Request comm_exmpl, User *usr_exmpl);
    response_server notice(Request comm_exmpl, User *usr_exmpl);
    response_server join(int fd_client, Request comm_exmpl, User *usr_exmpl);
	response_server who(Request comm_exmpl, User *usr_exmpl);
    response_server pingClient(int fd_client, Request comm_exmpl);
    response_server pongClient(Request comm_exmpl, User *usr_exmpl, int index_fd);
    response_server oper(Request comm_exmpl, User *usr_exmpl);
    response_server quit(Request comm_exmpl, User *usr_exmpl, int index_fd);
    response_server kill(Request comm_exmpl, User *usr_exmpl);
    response_server kick(Request comm_exmpl, User *usr_exmpl);
    response_server list(User *usr_exmpl);

    int getIndexFd(int fd);
    bool exit_server;
    int listen_socket;

    std::string _str_password;
    int password;
    int num;
    struct pollfd fd_list[MAX_USERS];
    std::vector<User*> _users;
    std::vector<Channel*> channels;
    std::time_t arr_timestamp[MAX_USERS];

    std::string _oper_user;
    std::string _oper_pass;

    User *getUser(int fd);
    std::vector<User*>::iterator getUserIter(int fd);
    User *getUser(std::string nick);
    int checkNick(std::string nick);
    int getCountCommand(char *buf);
    std::string getTmpBuf(int count, char *buf);

    void sendNoUser(int fd, std::string code, std::string text);
    Channel* getChannel(std::string channel_name);
    std::vector<User*> *getVectUser();
    void sendPrivChannel(std::vector<std::string> tmp_arg, std::string name_channel, std::string sender);
    void clearChannel(std::string name_user);
    void sendQuitUser(std::string name_user, std::vector<std::string> tmp_arg_1);

    std::string getMessageServ(std::vector<std::string> tmp_arg);
    std::string toLowerString(std::string strUp);

};

#endif //SERVER_HPP
