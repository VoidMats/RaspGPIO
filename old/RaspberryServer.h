#ifndef RASPBERRYSERVER_H
#define RASPBERRYSERVER_H
// C++ include
#include <exception>
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
#include <memory>
// My includes
#include "raspberry.h"

const int MAX_BUFFER{30000};

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

class MessagePacket
{
public:
    MessagePacket(const int& socket, const std::string& name):
        _socket{socket}, _name{name} {}
    ~MessagePacket() {}
    
    void setOutput(uint8_t pin) {
        std::string send{"SET_OUTPUT:" + _name + ":" + std::to_string(pin)};
        // Write command 
        //const char* send{"SET_OUTPUT:RASPBERRY:PIN"};  This is the old c way
        write(_socket, send.c_str(), send.length());
        // Get answer
        char buffer[MAX_BUFFER]{0};
        std::size_t recv_size = read(_socket, buffer, MAX_BUFFER);
        // regex to get OK, ERROR, TIMEOUT, WRONG SERVER
        if (recv_size > send.length()) {
            std::cout << "Output set: " << buffer << std::endl;
        }
    }
    void clrOutput(uint8_t pin) {
        std::cout << "This is the pin: " << pin << std::endl;
    }

private:
    //void clearBuffer() { memset(_buffer, 0, sizeof(_buffer)); }

    const int _socket;
    const std::string _name;
};

/*  Class used by the thread as callable object
*/
class ConnectedClient
{
public:
    ConnectedClient(const int& socket, std::shared_ptr<Raspberry> ptr):
        _socket{socket}, _ptr_gpio{ptr} {}
    void operator()() const
    {
        // Doing some stuff
        // TODO add lock to raspberry
        std::cout << "Thread has started - id: " << _socket << std::endl;
        char buffer[MAX_BUFFER]{0};
        std::size_t recv_size = read(_socket, buffer, MAX_BUFFER);
        int comp_res = strncmp(buffer, "SSH-2.0-OpenSSH_7.6p1 Ubuntu-4ubuntu0.3", 40);
        std::cout << "Compare result: " << comp_res << std::endl;
        if (comp_res == 13) {
            std::cout << "SSH connection: " << recv_size << " : " << buffer << std::endl;
            memset(buffer, 0, sizeof(buffer));
            recv_size = read(_socket, buffer, MAX_BUFFER);
        }
        std::cout << "Recived from client: " << buffer << std::endl;
        const char* answer{"Hello from server"};
        write(_socket, answer, strlen(answer));
        // We are done close this connection
        close(_socket);
    }
private:
    int _socket;
    std::shared_ptr<Raspberry> _ptr_gpio;
};

/* Main server class which uses the ConnectClient class
*/
class RaspberryServer
{
public:
    RaspberryServer(const std::string& name, std::shared_ptr<Raspberry> ptr_gpio);
    ~RaspberryServer();

    void start(const int& port);
    void closeConnection();

private:

    // Variables used for connection
    const int _backlog{5};
    int _socket_fd;
    bool _close{false};
    struct sockaddr_in _server_addr;
    // Variables used for GPIO
    std::string _server_name;
    std::shared_ptr<Raspberry> _ptr_gpio;
    // Not used yet
    std::vector<std::thread> _clients;
};

#endif //RASPBERRYSERVER_H