#ifndef FT_IRC_COMMAND_HPP
# define FT_IRC_COMMAND_HPP

//using namespace std;

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

class Command{
public:

    Command(const std::string buf);
    Command(const Command &copy);
    ~Command();

    Command &operator= (const Command &assign);

    const std::string get_comm() const;
private:
    Command();

    std::string _str_command;
    std::vector<std::string> _vect_args;
};

////CommandBook

class CommandBook{
public:
    CommandBook();
    CommandBook(const CommandBook &copy);
    ~CommandBook();
    CommandBook & operator= (const CommandBook &assign);
    void launchCommand(std::string command);

private:
//    std::map<std::string,
    void commandUser();

};
#endif
