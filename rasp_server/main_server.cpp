// C++ includes
#include <iostream>
#include <iomanip>
#include <thread>
// My classes
#include "raspberry.h"
#include "raspberryserver.h"
// Libraries
#include <bcm2835.h>

using namespace std;

int main()
{
    std::cout << "Server tester" << std::endl;
    const std::string raspberry_name{"test"};
    RaspberryServer server(raspberry_name, 2, "setup.txt", true, true);

    try {
        server.start(5050);
    }
    catch(ServerException& e) {
        std::cout << e.what() << std::endl;
    } 
 
    return 0;
}

