#ifndef BINARYFILE_H
#define BINARYFILE_H
// C++ include
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
// My include
#include "raspberrysetup.h"
#include "raspberry.h"
#include "serverexception.h"

class BinaryFile
{
public:
    BinaryFile(std::string filename): 
        _file_name{filename}, _file{} {};
    
    void writeSetup(std::vector<std::pair<uint8_t, PINMODE>> inSetup) 
    {
        // Open the file and write down the setup of the Raspberry
        _file.open(_file_name, std::ios::binary | std::ios::out);
        if (!_file) {
            std::string msg{"BinaryFile::writeSetup - File error <" + _file_name + ">"};
            throw ServerException(msg);
        }
        // Instansiate the RasberrySetup and add the Raspberry.h setting -> write to file
        RaspberrySetup setup;
        setup.setSetup(inSetup);
        setup.write(_file);
        _file.close();
    };
    
    std::vector<std::pair<uint8_t, PINMODE>> readSetup()
    {
        // open the file 
        _file.open(_file_name, std::ios::binary | std::ios::in);
        if(!_file)
        {
            std::string msg{"BinaryFile::writeSetup - File error <" + _file_name + ">"};
            throw ServerException(msg);
            //std::cerr << "File error " << _file_name << std::endl;
            //exit(EXIT_FAILURE);
        }
        RaspberrySetup setup; 
        setup.read(_file);
        _file.close();
        // Return the setting from file
        return setup.getSetup();
    };

private:
    std::string _file_name;
    std::fstream _file;
};

/*
void BinaryFile::writeSetup(std::vector<std::pair<uint8_t, PINMODE>> inSetup) {
  
}

std::vector<std::pair<uint8_t, PINMODE>> BinaryFile::readSetup() {
    
}
*/

#endif