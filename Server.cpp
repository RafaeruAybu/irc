#include "Server.hpp"

//init server
Serv::Serv(std::string port, std::string password)
{
    listen_socket = get_listen_sock(stoi(port));
    //todo delete
    std::cout << listen_socket << std::endl;
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
