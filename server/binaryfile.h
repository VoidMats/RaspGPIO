#ifndef BINARYFILE_H
#define BINARYFILE_H
// C++ include
#include <iostream>
#include <fstream>
// My includes
#include "raspberrypin.h"

class BinaryFile
{

public:
    BinaryFile(std::string filename): 
        _filename(filename) {};
    
    void writePin()
    {
        _file.open(_filename, std::ios::binary | std::ios::out);
        if (!_file) {
            std::cerr << "File error " << _filename << std::endl;
            exit(1);
        }
        RaspberryPin pin;
        pin.write(_file);
        _file.close();
    }

    RaspberryPin readPin()
    {
        File.open(FileName, std::ios::binary | std::ios::in);
        if(!File)
        {
            std::cerr<<"File error <"<<FileName<<">\n";
            exit(1);
        }
        Triangle trig; // default values
        trig.read(File);
        File.close();
        return trig;
    }

private:
    std::string _filename;
    std::fstream _file;
};

#endif