#ifndef MESSAGEPACKET_H
#define MESSAGEPACKET_H
// C++ include
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

/* Class to wrap the message between server and client */
class MessagePacket
{
public:
    MessagePacket(const int& socket, const std::string& name):
        _socket{socket}, _name{name} {}
    ~MessagePacket() {}
 
    bool setOutput(uint8_t pin);
    bool clrOutput(uint8_t pin);
    bool readInput(uint8_t pin, int& result);
    bool readTemp(uint8_t pin, float& result);
    bool readSecureTemp(uint8_t pin, float& result);

private:
    //void clearBuffer() { memset(_buffer, 0, sizeof(_buffer)); }
    std::vector<std::string> getSplitResult(std::string str); 

    const int _MAX_BUFFER{30000}; // TODO this has to be better done
    const int _socket;
    // Variables for saving data
    const std::string _name;
};

#endif //MESSAGEPACKET_H