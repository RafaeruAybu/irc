#include <iostream>
#include <string>
#include "../includes/Server.hpp"
#include "../includes/Utils.hpp"

int main(int argc, char *argv[])
{
    std::cout << "Hello world!!!" << std::endl; //todo delete line
    if (argc != 3)
    {
        std::cout << "Usage: ./ircserv <port> <password>" << std::endl;
        return 0;
    }
    try
    {
        Serv serv(ParseConfig(argv)); //new server
        serv.get_into_loop();
    }
    catch (std::exception &e)
    {
        std::cout << "Error during initialization: " << e.what() << std::endl;
    }
    //exit programm
    return 0;
}
