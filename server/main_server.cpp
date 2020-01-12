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
    int raspberry_model{2};
    bool raspberry_debug{false};
    bool gpio_debug{false};

    if (argc > 5) {
        printDefaulltArgv();
    }
    else if (argc == 5) {
        raspberry_name = argv[1];
        raspberry_model = std::stoi(argv[2]);
        gpio_debug = setBoolFromArgv(argv[3]);
        raspberry_debug = setBoolFromArgv(argv[4]);
    }
    else if (argc == 4) {
        printDefaulltArgv();
    }
    else if (argc == 3) {
        raspberry_name = argv[1];
        raspberry_model = std::stoi(argv[2]);
        std::string tmp;
        std::cout << "The server need to know its debug mode: " << std::endl;
        std::cin >> tmp;
        raspberry_debug = setBoolFromArgv(tmp);
        std::cout << "The Raspberry need to know its debug mode: " << std::endl;
        std::cin >> tmp;
        gpio_debug = setBoolFromArgv(tmp);
    }
    else if (argc == 2) {
        printDefaulltArgv();
    }
    else if( argc == 1 ) {
        std::cout << "Server is setup with default argument: " << std::endl;
        std::cout << "Name - test / Raspberry Model - 2B / Debug Server - true / Debug GPIO - true" << std::endl;
    }

    RaspberryServer server(raspberry_name, 
        raspberry_model, 
        "setup.txt", 
        raspberry_debug, 
        gpio_debug
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

void printDefaulltArgv() {
    std::cout << "Wrong incomming arguments. Please use program as:" << std::endl;
    std::cout << "rasp_server [name_server] [model_Raspberry] [debug_Raspberry] [debug_Server]" << std::endl;
    std::cout << "rasp_server [name_server] [model_Raspberry]" << std::endl;
}

