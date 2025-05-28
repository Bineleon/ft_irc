#include "../includes/Client.hpp"

Client::Client(){}


Client::~Client()
{

}

Client::Client(const Client& copy){ (void) copy; }

Client&	Client::operator=(const Client& copy)
{
	(void) copy;
	return *this;
}
