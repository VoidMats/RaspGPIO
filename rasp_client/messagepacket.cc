#include "messagepacket.h"

bool MessagePacket::setOutput(u_int8_t pin)
{
    // Write command and send
    std::ostringstream os;
    bool rtn_result{true};
    os << "SET_OUTPUT:" << _name << ":" << std::to_string(pin);
    std::string send{os.str()};
    std::cout << "Client sends: " << send << std::endl;
    write(_socket, send.c_str(), send.length());

    // Get answer from server
    char *buffer = new char[_MAX_BUFFER]();
    std::size_t recv_size = read(_socket, buffer, _MAX_BUFFER);
    // Check answer by splitting string into words
    std::string recv{buffer};
    std::cout << "Client Recieved: " << recv_size << " chars, " << recv << std::endl;
    std::vector<std::string> recv_split = getSplitResult(recv);

    //std::vector<std::string>::const_iterator it{recv_split.begin()};
    //for ( ; it!=recv_split.end(); ++it) {
    //    std::cout << *it;
    //}
    if (recv_split.at(0).compare("SET_OUTPUT") != 0) {
        std::cerr << "Wrong cmd" << std::endl;
        rtn_result = false;
    }
    if (recv_split.at(1).compare(_name) != 0) {
        std::cerr << "Wrong device name" << std::endl;
        rtn_result = false;
    }
    if (recv_split.at(2).compare(std::to_string(pin)) != 0) {
        std::cerr << "Wrong pin was set" << std::to_string(pin) << std::endl;
        rtn_result = false;
    }
    if (recv_split.at(3).compare("OK") != 0) {
        std::cerr << "Error code from device: " << recv_split.at(2) << std::endl;
        rtn_result = false;
    }
    // Clean and close connection
    delete[] buffer;
    //close(_socket); closing will be done by server
    return rtn_result;
}

bool MessagePacket::clrOutput(u_int8_t pin)
{
    // Write command 
    std::ostringstream os;
    bool rtn_result{true};
    os << "CLR_OUTPUT:" << _name << ":" << std::to_string(pin);
    std::string send{os.str()};
    std::cout << "Client sends: " << send << std::endl;
    write(_socket, send.c_str(), send.length());

    // Get answer from server
    char *buffer = new char[_MAX_BUFFER]();
    std::size_t recv_size = read(_socket, buffer, _MAX_BUFFER);
    // Check answer by splitting string into words
    std::string recv{buffer};
    std::cout << "Client Recieved: " << recv_size << " chars, " << recv << std::endl;
    std::vector<std::string> recv_split = getSplitResult(recv);

    if (recv_split.at(0).compare("CLR_OUTPUT") != 0) {
        std::cerr << "Client: Wrong cmd was recieved" << std::endl;
        rtn_result = false;
    }
    if (recv_split.at(1).compare(_name) != 0) {
        std::cerr << "Client: Wrong device name was recieved" << std::endl;
        rtn_result = false;
    }
    if (recv_split.at(2).compare(std::to_string(pin)) != 0) {
        std::cerr << "Client: Wrong pin was cleared" << std::to_string(pin) << std::endl;
        rtn_result = false;
    }
    if (recv_split.at(3).compare("OK") != 0) {
        std::cerr << "Client: Error code from device: " << recv_split.at(2) << std::endl;
        rtn_result = false;
    }
    // Clean and close connection
    delete[] buffer;
    //close(_socket); closing will be done by server
    return rtn_result;
}

bool MessagePacket::readInput(uint8_t pin, int& result)
{
    // Write command 
    std::ostringstream os;
    bool rtn_result{true};
    os << "READ_INPUT:" << _name << ":" << std::to_string(pin);
    std::string send{os.str()};
    std::cout << "Client sends: " << send << std::endl;
    write(_socket, send.c_str(), send.length());

    // Get answer from server
    char *buffer = new char[_MAX_BUFFER]();
    std::size_t recv_size = read(_socket, buffer, _MAX_BUFFER);
    // Check answer by splitting string into words
    std::string recv{buffer};
    std::cout << "Client Recieved: " << recv_size << " chars, " << recv << std::endl;
    std::vector<std::string> recv_split = getSplitResult(recv);
    
    if (recv_split.at(0).compare("READ_INPUT") != 0) {
        std::cerr << "Client: Wrong cmd was recieved" << std::endl;
        rtn_result = false;
    }
    if (recv_split.at(1).compare(_name) != 0) {
        std::cerr << "Client: Wrong device name was recieved" << std::endl;
        rtn_result = false;
    }
    if (recv_split.at(2).compare(std::to_string(pin)) != 0) {
        std::cerr << "Client: Wrong pin was cleared" << std::to_string(pin) << std::endl;
        rtn_result = false;
    }
    if (false) {  //std::stoi(recv_split.at(3)) != 0 || 1
        std::cerr << "Client: Error result from device: " << recv_split.at(2) << std::endl;
        rtn_result = false;
    }
    else {
        result = std::stoi(recv_split.at(3));
    }
    if (recv_split.at(3).compare("OK") != 0) {
        std::cerr << "Client: Error code from device: " << recv_split.at(2) << std::endl;
        rtn_result = false;
    }
    // Clean and close connection
    delete[] buffer;
    //close(_socket); closing will be done by server
    return rtn_result;
}

bool MessagePacket::readTemp(uint8_t pin, float& result)
{
    // Write command 
    std::ostringstream os;
    bool rtn_result{true};
    os << "READ_TEMP:" << _name << ":" << std::to_string(pin);
    std::string send{os.str()};
    std::cout << "Client sends: " << send << std::endl;
    write(_socket, send.c_str(), send.length());

    // Get answer from server
    char *buffer = new char[_MAX_BUFFER]();
    std::size_t recv_size = read(_socket, buffer, _MAX_BUFFER);
    // Check answer by splitting string into words
    std::string recv{buffer};
    std::cout << "Client Recieved: " << recv_size << " chars, " << recv << std::endl;
    std::vector<std::string> recv_split = getSplitResult(recv);
    
    if (recv_split.at(0).compare("READ_TEMP") != 0) {
        std::cerr << "Client: Wrong cmd was recieved" << std::endl;
        rtn_result = false;
    }
    if (recv_split.at(1).compare(_name) != 0) {
        std::cerr << "Client: Wrong device name was recieved" << std::endl;
        rtn_result = false;
    }
    if (recv_split.at(2).compare(std::to_string(pin)) != 0) {
        std::cerr << "Client: Wrong pin was cleared" << std::to_string(pin) << std::endl;
        rtn_result = false;
    }
    /* Check if the variable is float
    if (recv_split.at(3)  {
        std::cerr << "Client: Error code from device: " << recv_split.at(2) << std::endl;
        rtn_result = false;
    } else */
    result = std::stof(recv_split.at(4));

    if (recv_split.at(5).compare("OK") != 0) {
        std::cerr << "Client: Error code from device: " << recv_split.at(2) << std::endl;
        rtn_result = false;
    }
    // Clean and close connection
    delete[] buffer;
    //close(_socket); closing will be done by server
    return rtn_result;
}

bool MessagePacket::readSecureTemp(uint8_t pin, float& result)
{
    // Write command 
    std::ostringstream os;
    bool rtn_result{true};
    os << "READ_S_TEMP:" << _name << ":" << std::to_string(pin);
    std::string send{os.str()};
    std::cout << "Client sends: " << send << std::endl;
    write(_socket, send.c_str(), send.length());

    // Get answer from server
    char *buffer = new char[_MAX_BUFFER]();
    std::size_t recv_size = read(_socket, buffer, _MAX_BUFFER);
    // Check answer by splitting string into words
    std::string recv{buffer};
    std::cout << "Client Recieved: " << recv_size << " chars, " << recv << std::endl;
    std::vector<std::string> recv_split = getSplitResult(recv);
    
    if (recv_split.at(0).compare("READ_S_TEMP") != 0) {
        std::cerr << "Client: Wrong cmd was recieved" << std::endl;
        rtn_result = false;
    }
    if (recv_split.at(1).compare(_name) != 0) {
        std::cerr << "Client: Wrong device name was recieved" << std::endl;
        rtn_result = false;
    }
    if (recv_split.at(2).compare(std::to_string(pin)) != 0) {
        std::cerr << "Client: Wrong pin was cleared" << std::to_string(pin) << std::endl;
        rtn_result = false;
    }
    /* Check if the variable is float
    if (recv_split.at(3)  {
        std::cerr << "Client: Error code from device: " << recv_split.at(2) << std::endl;
        rtn_result = false;
    } else */
    result = std::stof(recv_split.at(4));

    if (recv_split.at(5).compare("OK") != 0) {
        std::cerr << "Client: Error code from device: " << recv_split.at(2) << std::endl;
        rtn_result = false;
    }
    // Clean and close connection
    delete[] buffer;
    //close(_socket); closing will be done by server
    return rtn_result;
}

std::vector<std::string> MessagePacket::getSplitResult(std::string str)
{
    std::vector<std::string> rtn_vector;
    std::stringstream ss(str);
    std::string word;

    while(std::getline(ss, word, ':'))
    {
        std::cout << "[" << word << "]";
        rtn_vector.push_back(word);
    }
    std::cout << std::endl;

    //std::regex re(":");
    //std::regex_token_iterator<std::string::iterator> it(str.begin(), str.end(), re, -1);
    //std::regex_token_iterator<std::string::iterator> end;
    //while (it != end) {
    //    std::cout << "[" << *it << "]";
    //    rtn_vector.push_back(*it);
    //}
    return rtn_vector;
}