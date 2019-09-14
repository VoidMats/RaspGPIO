#include <iomanip>
#include <iostream>
// My includes
#include "raspberryclient.h"
#include "messagepacket.h"

using namespace std;

void print_terminal();

int main(int argc, char *argv[])
{
    // This is not any nice coding. However, this is not really suppose to be used more 
    // then for testing. 

    int port{5050};
    string host{""};
    string name{"test"};

    if( argc > 4) {
        cerr << "To many arguments. Please use program as:" << endl;
        cerr << "rasp_client [host] [port] [raspGPIO_name]" << endl;
        return 1;
    }
    else if( argc == 4) {
        host = argv[1];
        port = stoi(argv[2]);
        name = argv[3];
    }
    else if( argc == 3 ) {
        cerr << "To few arguments. Please use program as:" << endl;
        cerr << "rasp_client [host] [port] [raspGPIO_name]" << endl;
        return 2;
    }
    else if( argc == 2) {
        host = argv[1];
    }
    else {
        cout << "Host, port and raspGPIO name is missing:" << endl;
        cout << "Please enter  host, port, name with space in between" << endl;
        cin >> host >> port >> name;
    }
    
    // Create variables needed
    RaspberryClient client(port, host);
    MessagePacket data(client.getSocket(), name);
    // Connect to server 
    client.connectToServer();

    // We loop the menu
    for(;;) {
        int cmd, pin;
        print_terminal();
        cin >> cmd >> pin;
        switch(cmd)
        {
            case 1:
            {
                bool error = data.setOutput(pin);
                cout << "Result from setOutput - pin " << pin << " : " << error << endl;
                break;
            }
            case 2:
            {
                bool error = data.clrOutput(pin);
                cout << "Result from clrOutput - pin " << pin << " : " << error << endl;
                break;
            }
            case 3:
            {
                int result{0};
                bool error = data.readInput(pin, result);
                cout << "Result from readInput - pin " << pin << " : " << error << " : " 
                        << result << endl;
                break;  
            }
            case 4:
            {
                float value{0.0};
                bool error = data.readTemp(pin, value);
                cout << "Result from readTemp - pin " << pin << " : " << error <<  " : " 
                        << value << endl;
                break;
            }
            case 5:
            {
                float value{0.0};
                bool error = data.readSecureTemp(pin, value);
                cout << "Result from readsecureTemp - pin " << pin << " : " << error <<  " : " 
                        << value << endl;
                break;
            }
            case 6:
            {
                cout << "Need to be implemented " <<  endl;
                break;
            }
            case 7:
            {
                cout << "Bye... " << endl;
                client.closeConnection();
                return 0;
            }
            default:
            {
                cerr << "Wrong command. Please retype";
                break;
            }
        }
    }


    /*  TODO move this to unit test - Test code 
    bool error = data.setOutput(17);
    cout << "Result from setOutput: " << error << endl;
    // Test setOutput one more time
    error = data.setOutput(22);
    cout << "Result2 from setOutput: " << error << endl;
    // Test clrOutput
    error = data.clrOutput(17);
    cout << "Result from clrOutput: " << error << endl;
    // Test readInput
    int i_result;
    error = data.readInput(18, i_result);
    cout << "Result from readInput: " << error << " " << i_result << endl;
    // Test readTemp
    float f_result;
    error = data.readTemp(4, f_result);
    cout << "Result from readTemp: " << error << " " << f_result << endl;
    // Test readSecureTemp
    error = data.readSecureTemp(4, f_result);
    cout << "Result from readSecureTemp: " << error << " " << f_result << endl;
    */
    
    // We should never reach this here. But in case....
    client.closeConnection();
    return 0;
}

void print_terminal()
{

    // We print the terminal
    cout << endl;
    cout << "client for raspGIO: " << endl;
    cout << "===================" << endl;
    cout << "Enter command + space + pin" << endl;
    cout << "1. Set output" << endl;
    cout << "2. Clear output" << endl;
    cout << "3. Read input" << endl;
    cout << "4. Read temperature" << endl;
    cout << "5. Read secure temperature" << endl;
    cout << "6. Read setup" << endl;
    cout << "7. Exit" << endl;
    cout << endl;
    cout << "Please enter command:" << endl;
}