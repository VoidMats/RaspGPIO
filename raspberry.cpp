#include "raspberry.h"

Raspberry::Raspberry(int model, bool debug)
    :model{model}, setup{false}, debug{debug}, errorCode{""},
    secureTemp{}, pins{}
{
    if( getuid() == 0) {
        bcm2835_set_debug(debug);
        if( !bcm2835_init() ) {
            std::cerr << "bcm2835 drivers can't initiate the Raspberry" << std::endl;
        }
    }
    else {
        std::cout << "bcm2835 drivers require that the program is run as sudo" << std::endl;
    }
}

/* Doxygen info
 *
 */
Raspberry::~Raspberry()
{
    bcm2835_close();
}

/* Variable mode is a enum 'bcm2835FunctionSelect' with values as:
 * BCM2835_GPIO_FSEL_INPT   0x00
 * BCM2835_GPIO_FSEL_OUTP   0x01
 * BCM2835_GPIO_FSEL_ALT0   0x04
 * BCM2835_GPIO_FSEL_ALT1   0x05
 * 
 * BCM2835_GPIO_FSEL_INPT   0x08 Added by this class to create secure temp
 * ...
 * See more information in doxygen for the drivers bcm2835 */
void Raspberry::setRPI(std::vector<std::pair<uint8_t, uint8_t>> list)
{
    std::vector<std::pair<uint8_t, uint8_t>>::const_iterator it{list.begin()};
    for( ; it!=list.end(); ++it ) {
        uint8_t pin = it->first;
        uint8_t mode = it->second;
        switch( mode )
        {
        case 0x00:
            bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_INPT);
            if( debug ) {
                std::cout << "Set pin: " << unsigned(pin) <<
                " to INPUT" << std::endl;
            }
            break;
        case 0x01:
            bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_OUTP);
            if( debug ) {
                std::cout << "Set pin: " << unsigned(pin) <<
                " to OUTPUT" << std::endl;
            }
            break;
        case 0x04:
            bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_ALT0);
            break;
        case 0x05:
            bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_ALT1);
            break;
        case 0x008:
            bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_INPT);
            secureTemp[pin] = -1000;
            if( debug ) {
                std::cout << "Set pin: " << unsigned(pin) <<
                " to SECURE TEMP" << std::endl;
            }
            break;
        default:
            break;
        }
        //bcm2835_gpio_fsel(pin, mode);

    }
    setup = true;
}

void Raspberry::setOutput(uint8_t pin)
{
    bcm2835_gpio_set(pin);
}

/* NB! This function is threaded and detached
 *
 */
void Raspberry::setOutputDelay(uint8_t pin, int ms)
{
    std::thread t(threadWriteDelayOutput, pin, HIGH, ms);
    t.detach();
}

/* NB! This function is threaded and detached
 *
 *
 */
void Raspberry::setOutUntilInput(uint8_t pinOut, uint8_t pinIn)
{
    std::thread t(threadSetOutUntilInput, pinOut, pinIn);
    t.detach();
}

void Raspberry::clrOutput(uint8_t pin)
{
    bcm2835_gpio_clr(pin);
    if( debug ) {
        std::cout << "Set pin: " << unsigned(pin) << " to LOW" << std::endl;
    }
}

/* This function does not care which state the pin has, it will just
 * be switched.
 *   */
void Raspberry::writeOutput(uint8_t pin, uint8_t value)
{
    if( value==0 && value==LOW ) {
        bcm2835_gpio_write(pin, LOW);
    }
    else if( value==1 && value==HIGH ) {
        bcm2835_gpio_write(pin, HIGH);
    }
}

/* NB! This function is threaded and detached
 * This function use a thread to switch a pin. The function does not
 * care which state of the pin it will just be switched. Use setOutputDelay
 * for forced low/high state.
 */
void Raspberry::writeOutputDelay(uint8_t pin, uint8_t value, int ms)
{
    std::thread t(threadWriteDelayOutput, pin, value, ms);
    t.detach();
}

uint8_t Raspberry::readInput(uint8_t pin)
{
    return bcm2835_gpio_lev(pin);
}

float Raspberry::readTemp(const uint8_t &pin, char unit, bool &error)
{
    float returnValue{0};
    error = false;
    if(setup)
    {
        if( presence(pin)==1 ) {
            std::cerr << "No device on pin: " << pin << std::endl;
            error = true;
            return -3000;
        }
        writeByte(pin, 0xCC);       // The Skip ROM command
        convert(pin);               // Convert command 0x44 - DS18B20 will start to measure
        presence(pin);
        writeByte(pin, 0xCC);
        writeByte(pin, 0xBE);

        int i;
        uint8_t data[9];
        for( i=0; i<9; i++ ) {
            data[i]=readByte(pin);
        }
        //std::cout << "Print the collected data" << std::endl;
        //for(const auto &b : data ) {
        //    std::cout << (unsigned)b << " " << std::endl;
        //}
        uint8_t crc=crc8(data, 9);  // CRC check
        if( crc!=0 ) {
            error = true;
            return -2000;
        }
        int t1 = data[0];
        int t2 = data[1];
        int16_t temp1 = (t2<<8|t1);   // Put the two bytes into a 16-bit integer
        float temp = static_cast<float>(temp1/16); // This is in Celcius

        // Check unit and recalculate if necessary
        if( unit=='C' || unit=='c' ) {
            returnValue = temp;
        }
        else if( unit=='F' || unit=='f' ) {
            temp = temp*1.8+32;
            returnValue = temp;
        }
        else {
            std::cerr << "Raspberry::readTemp() - Wrong unit has been given" << std::endl;
            error = true;
            return -1000;
        }
    }
    else {
        std::cerr << "Raspberry GPIO are not set" << std::endl;
        error = true;
        return -1000;
    }
    return returnValue;
}

float Raspberry::readSecureTemp(const uint8_t &pin, char unit, int limit)
{
    float _rtnValue;
    bool _error{true};
    uint8_t i{0};
    if( secureTemp[pin] == -1000 ) {
        secureTemp[pin] = readTemp(pin, unit, _error);
        _error = true;
    }
    // Check for errors. Try to read NUM_TMP_READ times
    while( _error && i<NUM_TMP_READ ) {
        _rtnValue = readTemp(pin, unit, _error);
        if( _error ) {
            bcm2835_delay(TIME_TMP_DELAY);
            std::cerr << "Found error" << std::endl;
        }
        i++;
    }
    i = 0;
    // Reading is out of range repeat
    while( _rtnValue>(secureTemp[pin] + limit) || 
           _rtnValue<(secureTemp[pin] - limit) ) 
    {
        _rtnValue = readTemp(pin, unit, _error);
        std::cerr << "Temperature out of range" << std::endl;
        if( i>= 5 ) break;
        i++;
    }
    secureTemp[pin] = _rtnValue;
    return _rtnValue;
}

void Raspberry::delayRPI(int value)
{
    bcm2835_delay(value);
}

void Raspberry::threadWriteDelayOutput(uint8_t const& pin, 
    uint8_t const& value, int ms)
{
    bool low{false};
    if( value==0 && value==LOW ) {
        bcm2835_gpio_write(pin, LOW);
        low = true;
    }
    else if( value==1 && value==HIGH ) {
        bcm2835_gpio_write(pin, HIGH);
    }
    bcm2835_delay(ms);
    if( low==true ) {
        bcm2835_gpio_write(pin, HIGH);
    } else {
        bcm2835_gpio_write(pin, LOW);
    }
}

void Raspberry::threadSetOutUntilInput(uint8_t const& pinOut, uint8_t const& pinIn)
{
    bcm2835_gpio_set(pinOut);
    for(;;) {
        if( bcm2835_gpio_lev(pinIn)==HIGH ) {
            bcm2835_gpio_clr(pinOut);
            break;
        }
        bcm2835_delayMicroseconds(TIME_SIGNAL_DELAY);
    }
}

int Raspberry::presence(uint8_t const& pin)
{
    //
    bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_write(pin, LOW);
    bcm2835_delayMicroseconds(480);
    bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_INPT);
    bcm2835_delayMicroseconds(70);
    uint8_t retValue = bcm2835_gpio_lev(pin);
    bcm2835_delayMicroseconds(410);
    return retValue;
}

void Raspberry::writeBit(const uint8_t &pin, int bit)
{
    int delay1, delay2;
    if(bit == 1) {
        delay1 = 6;
        delay2 = 64;
    } else {
        delay1 = 80;
        delay2 = 10;
    }
    bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_write(pin, LOW);
    bcm2835_delayMicroseconds(delay1);
    bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_INPT);
    bcm2835_delayMicroseconds(delay2);
}

void Raspberry::writeByte(const uint8_t &pin, int byte)
{
    for(int i = 0; i<8; i++) {
        if(byte & 1) {
            writeBit(pin,1);
        }
        else {
            writeBit(pin,0);
        }
        byte = byte >> 1;
    }
}

uint8_t Raspberry::readBit(const uint8_t &pin)
{
    bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_write(pin, LOW);
    bcm2835_delayMicroseconds(6);
    bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_INPT);
    bcm2835_delayMicroseconds(8);
    uint8_t retValue = bcm2835_gpio_lev(pin);
    bcm2835_delayMicroseconds(55);
    return retValue;
}

int Raspberry::readByte(const uint8_t &pin)
{
    int byte = 0;
    int i;
    for (i=0; i<8; i++) {
        byte = byte | readBit(pin) << i;
    };
    return byte;
}

int Raspberry::convert(const uint8_t &pin)
{
    int i;
    writeByte(pin, 0x44);
    for( i=0; i<1000; i++ ) {
        bcm2835_delayMicroseconds(100000);
        if( readBit(pin)==1 )
            break;
    }
    return i;
}

uint8_t Raspberry::crc8(const uint8_t *data, uint8_t len)
{
    uint8_t i, j, temp, databyte;

    uint8_t crc = 0;
    for( i = 0; i < len; i++) {
        databyte = data[i];
        for (j = 0; j < 8; j++) {
            temp = (crc ^ databyte) & 0x01;
            crc >>= 1;
            if (temp)
                crc ^= 0x8C;
            databyte >>= 1;
        }
    }
    return crc;
}
