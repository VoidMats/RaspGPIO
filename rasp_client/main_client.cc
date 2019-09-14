#include <iomanip>
#include <iostream>
// My includes
#include "raspberryclient.h"
#include "messagepacket.h"

using namespace std;

int main()
{   
    cout << "Test of client: " << endl;
    // Create variables needed
    RaspberryClient client(5050, "192.168.1.184");
    MessagePacket data(client.getSocket(), "test");
    // Test setOutput
    client.connectToServer();
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
    client.closeConnection();

    return 0;
}