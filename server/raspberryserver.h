#ifndef RASPBERRYSERVER_H
#define RASPBERRYSERVER_H
// C++ include
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <memory>
#include <regex>
#include <unordered_map>
#include <sstream>
#include <fstream>
// My includes
#include "raspberry.h"
#include "connectclient.h"
#include "binaryfile.h"
#include "serverexception.h"

using namespace std::chrono_literals;

/* Main server class which uses the ConnectClient class */
class RaspberryServer
{
public:
    RaspberryServer(
        const std::string& name, 
        int model, 
        std::string file, 
        bool debug_server=false, 
        bool debug_gpio=false);
    ~RaspberryServer();

    void start(const int& port);
    void closeConnection();
    std::string getServerName();
    int getServerPort();
    std::string getFileName();

private:
    void readSetup(std::string file_name);
    void writeSetup(std::string file_name);

    // Variables used for connection
    const int _backlog{5};
    int _socket_fd{0};
    int _port{0};
    struct sockaddr_in _server_addr{};
    bool _debugServer;
    bool _debugGPIO;
    // Variables used for GPIO
    std::string _file_name;
    std::string _server_name;
    std::shared_ptr<Raspberry> _ptr_gpio;
    std::mutex _mutex_gpio;
    BinaryFile _file_bin;
};

#endif //RASPBERRYSERVER_H