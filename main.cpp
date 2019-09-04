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

