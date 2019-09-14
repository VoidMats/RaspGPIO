#include "raspberryclient.h"

RaspberryClient::RaspberryClient(int port, std::string host):
    _port{port}, _host{host}, _server{NULL}, _socket_fd{0}, _server_addr{}
{
    // Get socket from server
    _socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket_fd < 0) {
        std::cerr << "Error socket" << strerror(errno) << std::endl;
    }

    // Reset _client_addr
    bzero(reinterpret_cast<char*>(&_server_addr), sizeof(_server_addr));

    // Bind values to addr
    memset(reinterpret_cast<char*>(&_server_addr), 0, sizeof(_server_addr));
    _server_addr.sin_family = AF_INET;
    _server_addr.sin_port = htons(_port);
    int check_ip = inet_pton(AF_INET, _host.c_str(), &_server_addr.sin_addr);
    if ( check_ip < 0)
        std::cerr << "Error IP address: " << strerror(errno) << std::endl;
}

//RaspberryClient::~RaspberryClient()
//{
//    delete _server;
//}

void RaspberryClient::connectToServer()
{
    // Connect to server
    int check_con = connect(_socket_fd, 
            reinterpret_cast<struct sockaddr*>(&_server_addr), 
            sizeof(_server_addr));
    if (check_con < 0)
        std::cerr << "Client: Error connection" << std::endl;

    std::cout << "Client::We have a connection: socket-" << _socket_fd << std::endl;

    //std::string send_msg{"SET_OUTPUT:test:3"};
    //write(_socket_fd, send_msg.c_str(), send_msg.length()); //there is alos send?? difference
}

int RaspberryClient::getSocket()
{
    return _socket_fd;
}   

void RaspberryClient::closeConnection(){
    std::cout << "Client: Closing connection: socket-" << _socket_fd << std::endl;
    close(_socket_fd);
}
