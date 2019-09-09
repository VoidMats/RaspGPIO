#ifndef CONNECTCLIENT_H
#define CONNECTCLIENT_H
// C++ include
#include <iostream>
#include <memory>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <vector>
#include <thread>
#include <unordered_map>
#include <sstream>
// My includes
#include "raspberry.h"

enum COMMANDS
{
    SET_OUTPUT = 0,
    CLR_OUTPUT = 1,
    READ_INPUT = 2,
    READ_TEMP = 3,
    READ_S_TEMP = 4
};

extern const std::unordered_map<std::string, COMMANDS> cmd_map;

/*  Class used by the thread as callable object */
class ConnectedClient
{
public:
    ConnectedClient(int socket, std::shared_ptr<Raspberry> ptr, const std::string& name):
        _socket{socket}, _ptr_gpio{ptr}, _name{name} {}

    void operator()() const;
    
private:
    static std::vector<std::string> getSplitResult(std::string str);
   
    const int _MAX_BUFFER{1024};
    const int _socket;
    std::shared_ptr<Raspberry> _ptr_gpio;
    const std::string _name;
};

#endif //CONNECTCLIENT_H

