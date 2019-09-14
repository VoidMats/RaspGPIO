#ifndef RASPBERRYCLIENT_H
#define RASPBERRYCLIENT_H
// C++ include
#include <exception>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
// My includes
#include "messagepacket.h"

/* This create a client which can talk to the RaspberryServer. 
 * At construction of the object port and host addess has to be given.
 * Connection is established by using the function connectToServer(). */
class RaspberryClient
{
public:
    RaspberryClient(int port, std::string host);
 //       _port{port}, _host{host}, _server{NULL}, _socket_fd{0}, _server_addr{} {}
    ~RaspberryClient() { delete _server; };
    RaspberryClient(RaspberryClient const& other) = default;
    RaspberryClient & operator = (RaspberryClient const& other) = default;
    RaspberryClient(RaspberryClient &&other) = default;
    RaspberryClient & operator = (RaspberryClient &&other) = default;

    void connectToServer();
    int getSocket();
    void closeConnection();

private:
    // Variables used for connection
    const int _port;
    const std::string _host;
    const hostent* _server;
    int _socket_fd;
    struct sockaddr_in _server_addr;
};

#endif //RASPBERRYCLIENT_H