#include "../includes/Command.hpp"


Command::Command(const std::string buf) {

    std::istringstream iss(buf);
    std::vector<std::string> results((std::istream_iterator<std::string>(iss)),
                                     std::istream_iterator<std::string>());

    if (results[0].length() > 0) {
        _str_command = results[0];
        results.erase(results.begin());
    }
    _vect_args = results;
    results.clear();
}

Command::Command(const Command &copy) : _str_command(copy._str_command), _vect_args(copy._vect_args) {

}

Command::~Command(){}

Command &Command::operator=(const Command &assign) {
    if (this == &assign)
        return *this;

    _str_command = assign._str_command;
    _vect_args = assign._vect_args;
    return (*this);
}

const std::string Command::get_comm() const {
    return _str_command;
}



////CommandBook

CommandBook::CommandBook() {

}

//CommandBook::CommandBook(const CommandBook &copy) {
//
//}

CommandBook::~CommandBook() {}

CommandBook &CommandBook::operator=(const CommandBook &assign) {
    if (this == &assign)
        return *this;
    return (*this);
}

//void CommandBook::launchCommand(std::string command) {
////    if (command == "USER")
//    void(command);
//}