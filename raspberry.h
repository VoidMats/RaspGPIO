#ifndef RASPBERRY_H
#define RASPBERRY_H
// C++ includes
#include <iostream>
#include <vector>
#include <map>
#include <iterator>
#include <string>
#include <thread>
#include <unistd.h>
// Library includes
#include <bcm2835.h>

//static int numThreads{0};

/* The main class for controlling the Raspberry Pi with bcm2835 drivers.
 * The bcm2835 drivers has to be installed and compiled with the class.
 *   */
class Raspberry
{
public:
    Raspberry(int model, bool debug);
    ~Raspberry();

    // Function to set the Raspberry
    void setRPI(std::vector<std::pair<uint8_t, uint8_t>> list);
    // Functions for get/send data
    void setOutput(uint8_t pin);
    void setOutputDelay(uint8_t pin, int ms);
    void setOutUntilInput(uint8_t pinOut, uint8_t pinIn);
    void clrOutput(uint8_t pin);
    void writeOutput(uint8_t pin, uint8_t value);
    void writeOutputDelay(uint8_t pin, uint8_t value, int ms);
    uint8_t readInput(uint8_t pin);
    float readTemp(const uint8_t &pin, char unit, bool &error);
    float readSecureTemp(const uint8_t &pin, char unit, int limit=5);
    void delayRPI(int value);

private:
    // Functions for the Raspberry Pi
    static void threadWriteDelayOutput(uint8_t const& pin, uint8_t const& value, int ms);
    static void threadSetOutUntilInput(uint8_t const& pinOut, uint8_t const& pinIn);
    int presence(const uint8_t &pin );
    void writeBit(const uint8_t &pin, int bit);
    void writeByte(const uint8_t &pin, int byte);
    uint8_t readBit(const uint8_t &pin );
    int readByte(const uint8_t &pin );
    int convert(const uint8_t &pin );
    uint8_t crc8(const uint8_t *data, uint8_t len );

    // Constant variable for process
    static const int NUM_TMP_READ = 5;
    static const int TIME_TMP_DELAY = 50;
    static const int TIME_SIGNAL_DELAY = 100;
    // Variables for the Raspberry Pi
    int model;
    bool setup;
    bool debug;
    std::string errorCode;
    std::map<int, float> secureTemp; // Contain the previous Temp mesurement
    std::vector<std::pair<uint8_t, std::string>> pins;
    // Variables for threads

};

#endif // RASPBERRY_H
