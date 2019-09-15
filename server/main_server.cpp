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

int main()
{
    std::cout << "Server tester" << std::endl;
    const std::string raspberry_name{"test"};
    RaspberryServer server(raspberry_name, 2, "setup.txt", true, true);

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

