#ifndef RASPBERRY_H
#define RASPBERRY_H
// C++ includes
#include <iostream>
#include <vector>
#include <iterator>
#include <string>
#include <thread>
// Library includes
#include <bcm2835.h>

using namespace std;

//static int numThreads{0};

/* The main class for controlling the Raspberry Pi with bcm2835 drivers.
 * The bcm2835 drivers has to be installed and compiled with the class.
 *
 *
 *
 * aksjdkajskaskdaksdnaksndkajsdkajsndkjansdknjkjasdknajsd */
class Raspberry
{
public:
    Raspberry(int model, bool debug);
    ~Raspberry();

    // Functions for get/send data
    void setRPI(vector<pair<uint8_t, uint8_t> > _list);
    void setOutput(uint8_t pin);
    void setOutUntilInput(uint8_t pinOut, uint8_t pinIn);
    void clrOutput(uint8_t pin);
    void writeOutput(uint8_t pin, uint8_t value);
    void writeOutputDelay(uint8_t pin, uint8_t value, int ms);
    uint8_t readInput(uint8_t pin);
    float readTemp(uint8_t pin, char unit, bool &error);
    float readSecureTemp(uint8_t pin, char unit);
    void delayRPI(int value);

private:
    // Functions for the Raspberry Pi
    void threadWriteDelayOutput(uint8_t pin, uint8_t value, int ms);
    void threadSetOutUntilInput(uint8_t pinOut, uint8_t pinIn);
    int presence( uint8_t _pin );
    void writeBit(uint8_t pin, int b );
    void writeByte( uint8_t _pin, int _byte );
    uint8_t readBit( uint8_t _pin );
    int readByte( uint8_t _pin );
    int convert( uint8_t _pin );
    uint8_t crc8(const uint8_t *data, uint8_t len );

    // Variables for the Raspberry Pi
    int model;
    bool setup;
    bool debug;
    string errorCode;
    float secureTemp;                   // Variable that contain the previous Temp mesurement
    vector<pair<uint8_t, string>> pins;
    // Variables for threads

};

#endif // RASPBERRY_H
