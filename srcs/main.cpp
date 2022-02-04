#include <iostream>
#include <string>
#include "Server.hpp"

int main(int argc, char *argv[])
{
    std::cout << "Hello world!!!" << std::endl; //todo delete line
    if (argc != 3)
    {
        std::cout << "Usage: ./ircserv <port> <password>" << std::endl;
        return 0;
    }
    std::string port(argv[1]); //convert cstyle to std::string
    std::string password(argv[2]); //same
    try
    {
        Serv serv(port, password); //new server
        serv.get_into_loop();
    }
    catch (std::exception &e)
    {
        std::cout << "Error during initialization: " << e.what() << std::endl;
    }
    //exit programm
    return 0;
}
