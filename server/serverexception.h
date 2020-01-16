#ifndef SERVEREXCEPTION_H
#define SERVEREXCEPTION_H
// C++ include
#include <exception>
#include <string>

/* Server excpetion */
class ServerException : std::exception 
{
public:
    ServerException(const std::string& msg):
        _msg{msg} {}

    virtual const char* what() const noexcept override
    {
        return _msg.c_str();
    }
private:
    std::string _msg;
};

#endif