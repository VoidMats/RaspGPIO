#include "raspberry.h"

Raspberry::Raspberry(int model, bool debug)
    :model{model}, debug{debug}
{
    bcm2835_set_debug(debug);
    if( !bcm2835_init() ) {
        cerr << "bcm2835 drivers can't initiate the Raspberry" << endl;
    }
    // TODO Need to add a check that the program is running as root
    // Ether if(getuid()) or CAP_SYS_NICE
    secureTemp = -1000;
    setup = false;
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
 * ...
 * See more information in doxygen for the drivers bcm2835 */
void Raspberry::setRPI(vector<pair<uint8_t, uint8_t>> _list)
{
    for( vector<pair<uint8_t, uint8_t>>::const_iterator it{_list.begin()};
         it!=_list.end(); ++it ) {
        uint8_t pin = it->first;
        uint8_t mode = it->second;
        switch( mode )
        {
        case 0x00:
            bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_INPT);
            if( debug ) { cout << "Set pin: " << unsigned(pin) << " to INPUT" << endl; }
            break;
        case 0x01:
            bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_OUTP);
            if( debug ) { cout << "Set pin: " << unsigned(pin) << " to OUTPUT" << endl; }
            break;
        case 0x04:
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

/* NB! This function has to be threaded and detached
 * ex.
 * std::thread t(&Raspberry::setOutputInput, &class, pinOut, pinIn);
 * thread.detach();
 */
void Raspberry::setOutUntilInput(uint8_t pinOut, uint8_t pinIn)
{
    //thread t(&Raspberry::threadSetOutUntilInput, this, pinOut, pinIn);
    //t.detach();
}

void Raspberry::clrOutput(uint8_t pin)
{
    bcm2835_gpio_clr(pin);
    if( debug ) { cout << "Set pin: " << unsigned(pin) << " to LOW" << endl; }
}

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
 *
 */
void Raspberry::writeOutputDelay(uint8_t pin, uint8_t value, int ms)
{
    //thread t(&Raspberry::threadWriteDelayOutput, this, pin, value, ms);
    //t.detach();
}

uint8_t Raspberry::readInput(uint8_t pin)
{
    return bcm2835_gpio_lev(pin);
}

float Raspberry::readTemp(uint8_t pin, char unit, bool &error)
{
    float returnValue{0};
    error = false;
    if(setup)
    {
        if( presence(pin)==1 ) {
            cerr << "No device on pin: " << pin << endl;
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
        //cout << "Print the collected data" << endl;
        //for(const auto &b : data ) {
        //    cout << (unsigned)b << " " << endl;
        //}
        uint8_t crc=crc8(data, 9);  // CRC check
        if( crc!=0 ) {
            error = true;
            return -2000;
        }
        int t1 = data[0];
        int t2 = data[1];
        int16_t temp1 = (t2<<8|t1);   // Put the two bytes into a 16-bit integer
        float temp = (float)temp1/16; // Recalculate to a floating point - This is in Celcius

        // Check unit and recalculate if necessary
        if( unit=='C' || unit=='c' ) {
            returnValue = temp;
        }
        else if( unit=='F' || unit=='f' ) {
            temp = temp*1.8+32;
            returnValue = temp;
        }
        else {
            cerr << "Raspberry::readTemp() - Wrong unit has been given" << endl;
            error = true;
            return -1000;
        }
    }
    else {
        cerr << "Raspberry GPIO are not set" << endl;
        error = true;
        return -1000;
    }
    return returnValue;
}

float Raspberry::readSecureTemp(uint8_t pin, char unit)
{
    float rtnValue;
    bool error{true};
    uint8_t i{0};
    if( secureTemp == -1000 ) {
        secureTemp = readTemp(pin, unit, error);
        error = true;
    }
    // Check for errors
    while( error && i<10 ) {
        rtnValue = readTemp(pin, unit, error);
        if( error ) { bcm2835_delay(50); cerr << "Found error" << endl; }
        i++;
    }
    i = 0;
    // Reading is out of range repeat
    while( rtnValue>secureTemp+5 || rtnValue<secureTemp-5 ) {
        rtnValue = readTemp(pin, unit, error);
        cerr << "Temperature out of range" << endl;
        if( i>= 10 ) break;
        i++;
    }
    secureTemp = rtnValue;
    return rtnValue;
}

void Raspberry::delayRPI(int value)
{
    bcm2835_delay(value);
}

void Raspberry::threadWriteDelayOutput(uint8_t pin, uint8_t value, int ms)
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
    }
    else {
        bcm2835_gpio_write(pin, LOW);
    }
}

void Raspberry::threadSetOutUntilInput(uint8_t pinOut, uint8_t pinIn)
{
    bool low{false};
    bcm2835_gpio_set(pinOut);
    while( low==false) {
        if( bcm2835_gpio_lev(pinIn)==HIGH ) {
            low = true;
            bcm2835_gpio_clr(pinOut);
        }
        bcm2835_delayMicroseconds(100);
    }
}

int Raspberry::presence(uint8_t pin)
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

void Raspberry::writeBit(uint8_t pin, int b)
{
    int delay1, delay2;
    if(b == 1) {
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

void Raspberry::writeByte(uint8_t _pin, int _byte)
{
    for(int i = 0; i<8; i++) {
        if(_byte & 1) {
            writeBit(_pin,1);
        }
        else {
            writeBit(_pin,0);
        }
        _byte = _byte >> 1;
    }
}

uint8_t Raspberry::readBit(uint8_t _pin)
{
    bcm2835_gpio_fsel(_pin, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_write(_pin, LOW);
    bcm2835_delayMicroseconds(6);
    bcm2835_gpio_fsel(_pin, BCM2835_GPIO_FSEL_INPT);
    bcm2835_delayMicroseconds(8);
    uint8_t retValue = bcm2835_gpio_lev(_pin);
    bcm2835_delayMicroseconds(55);
    return retValue;
}

int Raspberry::readByte(uint8_t _pin)
{
    int byte = 0;
    int i;
    for (i=0; i<8; i++) {
        byte = byte | readBit(_pin) << i;
    };
    return byte;
}

int Raspberry::convert(uint8_t pin)
{
    int i;
    writeByte( pin, 0x44);
    for( i=0; i<1000; i++ ) {
        bcm2835_delayMicroseconds(100000);
        if( readBit(pin)==1 )
            break;
    }
    return i;
}

uint8_t Raspberry::crc8(const uint8_t *data, uint8_t len)
{
    uint8_t i;
    uint8_t j;
    uint8_t temp;
    uint8_t databyte;

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
