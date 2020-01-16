#ifndef RASPBERRYSETUP_H
#define RASPBERRYSETUP_H
// C++ include
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>
// My include
#include "raspberrypin.h"
#include "raspberry.h"
/*
    RaspberrySetup is used to read and write Raspberry setting to a binary file.
    There is no internal check in this class. Before using the write() method make
    sure that the content is set by using setSetup(). The same is valid for getting
    a setting from the class. The content need to be read from file first. 
*/
class RaspberrySetup
{
public:
    RaspberrySetup():
    _gpio{} {
        for(int i=1; i<=MAX_PINS; i++) {
            _gpio.push_back(RaspberryPin{i, 0});
        }
    }
    // Public functions for reading/writting to file
    void write(std::ostream &os);
    void read(std::istream &is);
    // Public functions for getters/setters
    std::vector<std::pair<uint8_t, PINMODE>> getSetup() const;
    void setSetup(std::vector<std::pair<uint8_t, PINMODE>> inSetup);
private:
    std::vector<RaspberryPin> _gpio;
};

void RaspberrySetup::write(std::ostream &os) {
    std::vector<RaspberryPin>::iterator it{_gpio.begin()};
    for( ; it!=_gpio.end(); ++it) {
        it->write(os);
    }
}

void RaspberrySetup::read(std::istream &is) {
    std::vector<RaspberryPin>::iterator it{_gpio.begin()};
    for( ; it!=_gpio.end(); ++it) {
        it->read(is);
    }
}

std::vector<std::pair<uint8_t, PINMODE>> RaspberrySetup::getSetup() const {
    // Create a new Raspberry.h setting
    std::vector<std::pair<uint8_t, PINMODE>> rtn;
    // Loop through vector<RaspberryPin> and convert to Raspberry.h setting
    std::vector<RaspberryPin>::const_iterator it{_gpio.begin()};
    for( ; it!=_gpio.end(); ++it) {
        int no = it->getNo();
        int mode = it->getType();
        rtn.push_back(std::make_pair(no, static_cast<PINMODE>(mode)));  
    }
    return rtn;
}

void RaspberrySetup::setSetup(std::vector<std::pair<uint8_t, PINMODE>> inSetup) {
    // Clear the vector
    _gpio.clear();
    // Loop through incomming Raspberry.h setting and add to vector<RaspberryPin>
    std::vector<std::pair<uint8_t, PINMODE>>::const_iterator it{inSetup.begin()};
    for( ; it!=inSetup.end(); ++it) {
        int no = it->first;
        PINMODE mode = it->second;
        _gpio.push_back(RaspberryPin{no, static_cast<int>(mode)});  
    }
}   

#endif
