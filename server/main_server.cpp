// C++ includes
#include <iostream>
#include <iomanip>
#include <thread>
#include <exception>
// My classes
#include "raspberry.h"
#include "raspberryserver.h"
// Libraries
#include <bcm2835.h>

bool setBoolFromArgv(std::string arg);

int main(int argc, char *argv[])
{
    std::cout << "*** GPIO Server ***" << std::endl;
    std::string raspberry_name{"test"};
    bool raspberry_debug{true};
    bool gpio_debug{true};

    if (argc > 4) {
        std::cerr << "To many arguments. Please use program as:" << std::endl;
        std::cerr << "rasp_server [debug_Raspberry] [debug_Server] [name_server]" << std::endl;
    }
    else if (argc == 4) {
        raspberry_debug = setBoolFromArgv(argv[1]);
        gpio_debug = setBoolFromArgv(argv[2]);
        raspberry_name = argv[3];
    }
    else if (argc == 3) {
        raspberry_debug = setBoolFromArgv(argv[1]);
        gpio_debug = setBoolFromArgv(argv[2]);
        std::cout << "Server name - test" << std::endl;
    }
    else if (argc == 2) {
        std::string tmp;
        std::cerr << "The server need to know its debug mode:" << std::endl;
        std::cin >> tmp;
        gpio_debug = setBoolFromArgv(tmp);
        raspberry_debug = gpio_debug; 
    }
    else if( argc == 1 ) {
        std::cout << "Server is setup with default argument: " << std::endl;
        std::cout << "Name - test / Debug Server - true / Debug GPIO - true" << std::endl;
    }

    // TODO set mode to
    RaspberryServer server(raspberry_name, 2, 
        "setup.txt", 
        raspberry_debug, 
        raspberry_debug
    );

    try {
        server.start(5050);
    }
    catch(const ServerException& e) {
        std::cout << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        throw;
    }
 
    return 0;
}

bool setBoolFromArgv(std::string arg) {
    return ( arg == "true" || arg == "True" ) ? true : false;
}

