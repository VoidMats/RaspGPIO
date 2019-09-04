#include "raspberryclient.h"

RaspberryClient::RaspberryClient(int port, std::string host):
    _port{port}, _host{host}
{
    _server = gethostbyname(host.c_str());
    
}

RaspberryClient::~RaspberryClient()
{

}