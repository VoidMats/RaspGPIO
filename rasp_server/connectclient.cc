#include "connectclient.h"

const std::unordered_map<std::string, COMMANDS> cmd_map = {
    {"SET_OUTPUT", COMMANDS::SET_OUTPUT},
    {"CLR_OUTPUT", COMMANDS::CLR_OUTPUT},
    {"READ_INPUT", COMMANDS::READ_INPUT},
    {"READ_TEMP", COMMANDS::READ_TEMP},    
    {"READ_S_TEMP", COMMANDS::READ_S_TEMP}    
};

void ConnectedClient::operator()() const
{
    std::cout << "Server::Thread has started - id: " << std::this_thread::get_id() << std::endl;
    //char data;
    //recv(_socket, &data,1, MSG_PEEK);
    //std::cout <<"Data: " << data << std::endl;
    for(;;) 
    {
        //std::shared_ptr<char[]> buffer = std::make_shared<char[]>(_MAX_BUFFER);
        char *buffer = new char[_MAX_BUFFER]();
        std::size_t recv_size = read(_socket, buffer, _MAX_BUFFER);
        std::cout << "Server::Read the incomming call from buffer: " << recv_size << " words:" << buffer << std::endl;
        // Check if connection is closed. If zero bytes are recv client has closed the connection
        if (recv_size == 0) {
            std::cout << "Server: 0 byte found going out of loop" << std::endl;
            delete[] buffer;
            break; // break out of for-loop
        }
        const std::string recv(buffer, recv_size);
        // Check cmd and get attributes
        std::vector<std::string> recv_split;;
        std::stringstream ss(recv);
        std::string word;
        while(std::getline(ss, word, ':'))
        {
            std::cout << "[" << word << "]";
            recv_split.push_back(word);
        }
        //std::vector<std::string> recv_split = getSplitResult(recv);
        std::string cmd = recv_split.at(0);
        std::string name = recv_split.at(1);
        uint8_t pin = static_cast<uint8_t>(std::stoi(recv_split.at(2)));
        // Run command
        std::ostringstream os;
        std::string answer;
        switch ( cmd_map.at(cmd) )
        {
            case COMMANDS::SET_OUTPUT:
            {
                os << "SET_OUTPUT:" + _name + ":" << std::to_string(pin);
                _mutex_gpio.lock();
                _ptr_gpio->setOutput(pin);
                _mutex_gpio.unlock();
                os << ":OK";
                answer = os.str();
                break;
            }
            case COMMANDS::CLR_OUTPUT:
            {
                os << "CLR_OUTPUT:" + _name + ":" << std::to_string(pin);
                _ptr_gpio->clrOutput(pin);
                os << ":OK";
                answer = os.str();
                break;
            }
            case COMMANDS::READ_INPUT:
            {
                os << "READ_INPUT:" + _name + ":" << std::to_string(pin);
                //answer = "READ_INPUT:" + _name + ":" + std::to_string(pin);
                _mutex_gpio.lock();
                uint8_t value = _ptr_gpio->readInput(pin);
                _mutex_gpio.unlock();
                os << ":" << std::to_string(value) << ":OK";
                answer = os.str();
                break;
            }
            case COMMANDS::READ_TEMP:
            {
                os << "READ_TEMP:" + _name + ":" << std::to_string(pin);
                bool error{false};
                _mutex_gpio.lock();
                float value = _ptr_gpio->readTemp(pin, 'C', error);
                _mutex_gpio.unlock();
                if (!error) 
                    os << ":C:-257.14:ERROR";
                else 
                    os << ":C:" << std::to_string(value) << ":OK"; // setprecision
                answer = os.str();
                break;
            }
            case COMMANDS::READ_S_TEMP:
            {
                os << "READ_S_TEMP:" << _name << std::to_string(pin);
                _mutex_gpio.lock();
                float value = _ptr_gpio->readSecureTemp(pin, 'C');
                _mutex_gpio.unlock();
                os << ":C:" << std::to_string(value) << ":OK";
                answer = os.str();
                break;
            }
            default:
            {
                os << cmd << ":" << _name << ":NO_CMD_FOUND";
                answer = os.str();
                break;
            }
        }
        write(_socket, answer.c_str(), answer.length());
        std::cout << "Server answer: " << answer << std::endl;

        // We are done.... let other threads run
        std::this_thread::yield();

        // Give some time for client to recive msg
        //std::this_thread::sleep_for(1s);

        // We are done with this command clean_up
        delete[] buffer;
    }

    // Clean_up of the call
    std::lock_guard<std::mutex> lock(_mutex_gpio); // destructor
    close(_socket);  // make sure the connection is closed, even if the client is primary source of closing
    std::cout << "Thread will die - id: " << std::this_thread::get_id() << std::endl;
}

std::vector<std::string> getSplitResult(std::string str)
{
    std::vector<std::string> rtn_vector;
    std::stringstream ss(str);
    std::string word;

    while(std::getline(ss, word, ':'))
    {
        std::cout << "[" << word << "]";
        rtn_vector.push_back(word);
    }
    //std::cout << std::endl;

    return rtn_vector;
}


