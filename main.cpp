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
std::vector<pair<uint8_t, uint8_t>> setupGPIO();

int main()
{
    std::cout << "Server tester" << std::endl;
    const std::string raspberry_name{"test"};
    std::shared_ptr<Raspberry> ptr_GPIO;
    auto setup = setupGPIO();
    ptr_GPIO->setRPI(setup);
    RaspberryServer server(raspberry_name, ptr_GPIO);

    try {
        server.start(5050);
    }
    catch(ServerException& e) {
        std::cout << e.what() << std::endl;
    } 
 
    // cout << "BCM2835 tester" << endl;
    /*
    uint8_t debug{0};
    uint8_t counter{4};
    //bool error{false};
    Raspberry test(2, debug);
    
    test.setRPI(setup);
    while( counter>0 ){
        if( test.readInput(18)==HIGH ) {
            test.delayRPI(1500);
            test.writeOutputDelay(23,HIGH,1500);
            test.setOutput(17);
            test.writeOutput(22,HIGH);
            test.delayRPI(1500);
            test.clrOutput(17);
            test.writeOutput(22,LOW);
            test.delayRPI(1500);
            test.setOutput(17);
            test.delayRPI(1500);
            test.clrOutput(17);
            test.delayRPI(1500);
            test.setOutput(17);
            test.delayRPI(1500);
            test.clrOutput(17);
            counter--;

        }
        float value = test.readSecureTemp(4, 'C');
        cout << "Temperature: " << setprecision(3) << setw(6) << value << " C" << endl;
        test.delayRPI(100);
    }
    */
    return 0;
}

std::vector<pair<uint8_t, uint8_t>> setupGPIO()
{
    std::vector<pair<uint8_t, uint8_t>> setup;
    setup.push_back(make_pair(4,0));  // Set GPIO4 to input
    setup.push_back(make_pair(17,1)); // set GPIO17 to output
    setup.push_back(make_pair(22,1)); // Set GPIO22 to output
    setup.push_back(make_pair(23,1)); // Set GPIO23 to output
    setup.push_back(make_pair(18,0)); // Set GPIO18 to input
    return setup;
}
