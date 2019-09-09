#include "raspberryserver.h"


RaspberryServer::RaspberryServer(const std::string& name, int model, std::string file,
    bool debug_server, bool debug_gpio):
    _debug{debug_server}, 
    _file_name{file}, 
    _server_name{name},
    _ptr_gpio{std::make_shared<Raspberry>(model, debug_gpio)}
{
    
}

RaspberryServer::~RaspberryServer()
{

}

/* Create socket and bind value to socket. If all is OK listen on that socket.

    struct sockaddr_in
    {
        __uint8_t       sin_len;    //
        sa_family_t     sin_family; // The address family
        in_port_t       sin_port;   // The port number
        struct in_addr  sin_addr;   // The address for this socket. IP address
        char            sin_zero[8];
    };
*/
void RaspberryServer::start(const int& port)
{
    // Read setup file and configure the Raspberry Pi
    readSetup(_file_name);

    _socket_fd = 0;

    // *** Create socket ***
    _socket_fd = socket(AF_INET, SOCK_STREAM, 0);  // socket(domain, type, protocol)
    if (_socket_fd < 0) {
        throw ServerException("Can't open the socket");
    }

    // Clear server_addr. It should be clear but in case
    bzero(reinterpret_cast<char*>(&_server_addr), sizeof(_server_addr)); //(char*)

    // Set socket for reuse (otherwice we might need to wait for socket to close)
    int option{1};
    setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option, sizeof(option));

    // *** Bind socket to a IP / port ***
    memset(reinterpret_cast<char*>(&_server_addr), 0, sizeof(_server_addr));
    _server_addr.sin_family = AF_INET;
    _server_addr.sin_addr.s_addr = htonl(INADDR_ANY);   // htonl converts a long integer to a network representation
    _server_addr.sin_port = htons(port);                // htons converts a short integer to a network representation 

    // bind(int socket, const struct sockaddr *address, socklen_t address_len)
    int check_bind = bind(_socket_fd, reinterpret_cast<struct sockaddr*>(&_server_addr), sizeof(_server_addr));
    if (check_bind < 0 ) {
        throw ServerException("Can't bind to IP or port");
    }

    // *** Listen to incomming calls ***
    if (listen(_socket_fd, _backlog) < 0) {
        throw ServerException("Can't listen on socket");
    }
    else {
        // *** Accept a call ***
        // int accept( int socket , struct sockaddr* restrict address, socklen_t* restrict address_len)

        std::cout << "Listening to port: " << port << "....." << std::endl;
        // TODO add a proper stop instead of for(;;)
        for (;;) 
        {
            struct sockaddr_in _client_addr;
            int client_size{sizeof(_client_addr)};
            int new_socket_fd = 0;
            new_socket_fd = accept(_socket_fd,
                                   reinterpret_cast<struct sockaddr*>(&_client_addr),
                                   reinterpret_cast<socklen_t*>(&client_size));
            if (new_socket_fd < 0) {
                std :: cerr << "Can't accept connection" << std::endl;
                //throw ServerException("Can't accept connection"); // This shouldn't be an exception
            }
            else {
                std::cout << "create connection with client..." << std::endl;
                ConnectedClient new_client{new_socket_fd, _ptr_gpio, _server_name};
                std::thread t(new_client);
                t.detach();
            }
        }
        // *** Closing server connection ***
        close(_socket_fd);
    }    
}

void RaspberryServer::closeConnection()
{
    close(_socket_fd);
}

void RaspberryServer::readSetup(std::string file_name)
{
    std::vector<std::pair<uint8_t, uint8_t>> setup;
    // Open file and read content
    std::ifstream file(file_name);
    if (file.is_open()) {
        int line_no{1};
        std::string line;
        while (getline(file, line)) {
            std::istringstream is(line);
            int pin, mode;
            if (!(is >> mode >> pin)) { 
                std::string msg{"RaspberryServer::readSetup - Parse error on line "};
                msg += std::to_string(line_no);
                throw ServerException(msg);
            }
            setup.push_back(std::make_pair(pin, mode));  
            if (_debug) 
                std::cout << "parse line: " << std::to_string(line_no) << " - " 
                << std::to_string(pin) << " " << std::to_string(mode) << std::endl;
            //static_cast<std::underlying_type<PINMODE>::type>(mode)
            line_no++;
        }
    }
    else {
        std::string msg{"RaspberryServer::readSetup - Can't open file " + file_name};
        throw ServerException(msg);
    }
    file.close();
    // Set input values to Raspberry class
    _ptr_gpio->setRPI(setup);
}
