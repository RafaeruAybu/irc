#include <iostream>
#include <string>
// #include "Server.hpp"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cout << "Usage: ./ircserv <port> <password>" << std::endl;
        return 0;
    }
    std::string port(argv[1]); //convert cstyle to std::string
    std::string password(argv[2]); //same
    // Serv(port, password); //new server
    //while ??

    //exit programm
    return 0;
}
