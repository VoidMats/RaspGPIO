#ifndef RASPBERRYPIN_H
#define RASPBERRYPIN_H
// C++ include
#include <iostream>
#include <fstream>

class RaspberryPin
{
public:
    RaspberryPin():
        _gpio_no{0},
        _gpio_type{0} {}
    RaspberryPin(
        int gpio_no,
        int gpio_type
    ):
    _gpio_no{gpio_no},
    _gpio_type{gpio_type} {}

    void write(std::ostream &os);
    void read(std::istream &is);
private:
    int _gpio_no;
    int _gpio_type;
};

void RaspberryPin::write(std::ostream &os) {
    os.write( (char*)&_gpio_no, sizeof(_gpio_no) );
    os.write( (char*)&_gpio_type, sizeof(_gpio_type) );
}

void RaspberryPin::read(std::istream &is) {
    is.read( (char*)&_gpio_no, sizeof(_gpio_no) );
    is.read( (char*)&_gpio_no, sizeof(_gpio_type) );
}

#endif