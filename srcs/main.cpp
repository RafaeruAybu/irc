#include <iostream>
#include <string>
#include "../includes/Server.hpp"

int main(int argc, char *argv[]) {
    std::cout << "Hello world!!!" << std::endl;
    if (argc != 3) {
        std::cout << "Usage: ./ircserv <port> <password>" << std::endl;
        return 0;
    }
    try {
        Serv serv(argv[1], argv[2]);
        serv.get_into_loop();
    }
    catch (std::exception &e) {
        std::cout << "Error during initialization: " << e.what() << std::endl;
    }
    return 0;
}
