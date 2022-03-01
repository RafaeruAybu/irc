#include "../includes/Request.hpp"


Request::Request(const std::string buf) : _prefix("") {

    std::istringstream iss(buf);
    std::vector<std::string> results((std::istream_iterator<std::string>(iss)),
                                     std::istream_iterator<std::string>());

    if (results[0].length() > 0) {
        if (results[0][0] == ':') {
            _prefix = std::string(results[0].begin() + 1, results[0].end());
            results.erase(results.begin());
        }
        _str_command = results[0];
        std::cout << "Request str_command:" << _str_command << "\n";
        results.erase(results.begin());
    }
    _vect_args = results;
    results.clear();
}

Request::Request(const Request &copy) : _str_command(copy._str_command), _vect_args(copy._vect_args) {

}

Request::~Request(){}

Request &Request::operator=(const Request &assign) {
    if (this == &assign)
        return *this;

    _str_command = assign._str_command;
    _vect_args = assign._vect_args;
    return (*this);
}

const std::string Request::get_comm() const {
    return _str_command;
}

const std::vector<std::string>& Request::get_vect_arg() const {
    return (_vect_args);
}


