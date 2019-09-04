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
#include <chrono>
#include <memory>
#include <regex>
#include <unordered_map>
#include <sstream>
#include <fstream>
// My includes
#include "raspberry.h"

using namespace std::chrono_literals;

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
    void operator()() const
    {
        // TODO add lock to raspberry
        std::cout << "Server::Thread has started - id: " << std::this_thread::get_id() << std::endl;
        //char data;
        //recv(_socket, &data,1, MSG_PEEK);
        //std::cout <<"Data: " << data << std::endl;
        for(;;) 
        {
            //std::shared_ptr<char[]> buffer = std::make_shared<char[]>(_MAX_BUFFER);
            char *buffer = new char[_MAX_BUFFER]();
            std::size_t recv_size = read(_socket, buffer, _MAX_BUFFER);
            std::cout << "Server::Read the incomming call from buffer: " << recv_size << " words:" << buffer << std::endl;
            // Check if connection is closed. If zero bytes are recv client has closed the connection
            if (recv_size == 0) {
                std::cout << "Server: 0 byte found going out of loop" << std::endl;
                delete[] buffer;
                break; // break out of for-loop
            }
            std::string recv(buffer, recv_size);
            // Check cmd and get attributes
            std::vector<std::string> recv_split = getSplitResult(recv);
            std::string cmd = recv_split.at(0);
            std::string name = recv_split.at(1);
            uint8_t pin = static_cast<uint8_t>(std::stoi(recv_split.at(2)));
            // Run command
            std::ostringstream os;
            std::string answer;
            switch ( cmd_map.at(cmd) )
            {
                case COMMANDS::SET_OUTPUT:
                {
                    os << "SET_OUTPUT:" + _name + ":" << std::to_string(pin);
                    _ptr_gpio->setOutput(pin);
                    os << ":OK";
                    answer = os.str();
                    break;
                }
                case COMMANDS::CLR_OUTPUT:
                {
                    os << "CLR_OUTPUT:" + _name + ":" << std::to_string(pin);
                    _ptr_gpio->clrOutput(pin);
                    os << ":OK";
                    answer = os.str();
                    break;
                }
                case COMMANDS::READ_INPUT:
                {
                    os << "READ_INPUT:" + _name + ":" << std::to_string(pin);
                    //answer = "READ_INPUT:" + _name + ":" + std::to_string(pin);
                    uint8_t value = _ptr_gpio->readInput(pin);
                    os << ":" << std::to_string(value) << ":OK";
                    answer = os.str();
                    break;
                }
                case COMMANDS::READ_TEMP:
                {
                    os << "READ_TEMP:" + _name + ":" << std::to_string(pin);
                    bool error{false};
                    float value = _ptr_gpio->readTemp(pin, 'C', error);
                    if (!error) 
                        os << ":C:-257.14:ERROR";
                    else 
                        os << ":C:" << std::to_string(value) << ":OK"; // setprecision
                    answer = os.str();
                    break;
                }
                case COMMANDS::READ_S_TEMP:
                {
                    os << "READ_S_TEMP:" << _name << std::to_string(pin);
                    float value = _ptr_gpio->readSecureTemp(pin, 'C');
                    os << ":C:" << std::to_string(value) << ":OK";
                    answer = os.str();
                    break;
                }
                default:
                {
                    os << cmd << ":" << _name << ":NO_CMD_FOUND";
                    answer = os.str();
                    break;
                }
            }
            write(_socket, answer.c_str(), answer.length());
            std::cout << "Server answer: " << answer << std::endl;

            // Give some time for client to recive msg
            //std::this_thread::sleep_for(1s);

            // We are done with this command clean_up
            delete[] buffer;
        }
        
        close(_socket);  // make sure the connection is closed, even if the client is primary source of closing
        std::cout << "Thread will die - id: " << std::this_thread::get_id() << std::endl;
    }
private:
    std::vector<std::string> getSplitResult(std::string str) const {
        
        std::vector<std::string> rtn_vector;
        std::stringstream ss(str);
        std::string word;

        while(std::getline(ss, word, ':'))
        {
            std::cout << "[" << word << "]";
            rtn_vector.push_back(word);
        }
        std::cout << std::endl;

        return rtn_vector;
    }

    const int _MAX_BUFFER{1024};
    const int _socket;
    std::shared_ptr<Raspberry> _ptr_gpio;
    const std::string _name;
};

/* Main server class which uses the ConnectClient class */
class RaspberryServer
{
public:
    RaspberryServer(const std::string& name, int model, 
        bool debug_server = false, bool debug_gpio = false);
    ~RaspberryServer();

    void start(const int& port);
    void closeConnection();

private:
    void readSetup();

    // Variables used for connection
    const int _backlog{5};
    int _socket_fd;
    bool _close{false};
    struct sockaddr_in _server_addr;
    // Variables used for GPIO
    std::string _server_name;
    bool _debug;
    std::shared_ptr<Raspberry> _ptr_gpio;
};

#endif //RASPBERRYSERVER_H