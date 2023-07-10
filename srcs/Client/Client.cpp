/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcorenti <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 02:35:26 by dcorenti          #+#    #+#             */
/*   Updated: 2023/07/05 00:46:01 by dcorenti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client()
{
	memset(&_clientAddress, 0, sizeof(_clientAddress));
	_clientAddressLenght = sizeof(_clientAddressLenght);
	_lastTime = time(NULL);
	_active = true;
	_bodySize = 0;
	_requestDone = false;
}

Client::Client(const Client& copy)
{
	_sockfd = copy._sockfd;
	_servfd = copy._servfd;
	_clientAddress = copy._clientAddress;
	_clientAddressLenght = copy._clientAddressLenght;
	_dataRecv = copy._dataRecv;
	_lastTime = copy._lastTime;
	_active = copy._active;
	_bodySize = copy._bodySize;
	_request = copy._request;
	_requestDone = copy._requestDone;
}

Client::~Client()
{

}

Client&	Client::operator=(const Client& copy)
{
	if (this != &copy)
	{
		_sockfd = copy._sockfd;
		_servfd = copy._servfd;
		_clientAddress = copy._clientAddress;
		_clientAddressLenght = copy._clientAddressLenght;
		_dataRecv = copy._dataRecv;
		_lastTime = copy._lastTime;
		_active = copy._active;
		_bodySize = copy._bodySize;
		_request = copy._request;
		_requestDone = copy._requestDone;
	}
	return(*this);

}


/*-----------SETTERS------------*/

void	Client::setSockfd(int fd)
{
	_sockfd = fd;
}

void	Client::setServfd(int fd)
{
	_servfd = fd;
}

void	Client::setClientAddress(struct sockaddr_in clientAddress)
{
	_clientAddress = clientAddress;
}

void	Client::setClientAdressLenght(socklen_t clientAddressLenght)
{
	_clientAddressLenght = clientAddressLenght;
}

void	Client::setDataRecv(std::string data)
{
	_dataRecv += data;
}

void	Client::setLastTime()
{
	_lastTime = time(NULL);
}

void	Client::setInactive()
{
	_active = false;
}

void 	Client::setRequest(Request& request)
{
	_request = request;
}

void	Client::setRequestDone(bool value)
{
	_requestDone = value;
}

/*-----------GETTERS------------*/

int 			Client::getSockfd() const
{
	return(_sockfd);
}

int 			Client::getServfd() const
{
	return(_servfd);
}

struct sockaddr_in	Client::getClientAddress() const
{
	return(_clientAddress);
}

socklen_t 		Client::getClientAddressLenght() const
{
	return(_clientAddressLenght);
}

std::string		Client::getDataRecv() const
{
	return(_dataRecv);
}

time_t			Client::getLastTime() const
{
	return(_lastTime);
}

bool			Client::getActive() const
{
	return(_active);
}

unsigned long	Client::getBodySize() const
{
	return(_bodySize);
}

Request		Client::getRequest() const
{
	return(_request);
}

bool		Client::getRequestDone() const
{
	return(_requestDone);
}

/*--------MEMBER FUNCTION--------*/

bool			Client::isHTTPrequest()
{
	if (!_dataRecv.empty() && strstr(_dataRecv.c_str(), "HTTP") != NULL)
		return(true);
	else
		return(false);
}

bool			Client::requestFilled()
{
	return(true);
}

void			Client::closefd()
{
	close(_sockfd);
}

bool			Client::allDataReceive()
{
	if (!_dataRecv.empty())
	{
		const char *end_headers = strstr(_dataRecv.c_str(), "\r\n\r\n");
		if (end_headers != NULL)
		{
			const char *content_lenght = strstr(_dataRecv.c_str(), "Content-Length:");
			if (content_lenght != NULL)
			{
				setBodySize(content_lenght);
			}
			return(true);
		}

	}
	return(false);
}

void			Client::setBodySize(std::string content_lenght_line)
{
	std::size_t index = content_lenght_line.find_first_of("0123456789");
	std::string bodysize;

	while (index < content_lenght_line.size() && content_lenght_line[index] != '\r')
	{
		bodysize += content_lenght_line[index];
		index++;
	}
	index = 0;
	while(index < bodysize.size())
	{
		_bodySize *= 10;
		_bodySize += bodysize[index];
		index++;
	}
}

bool			Client::checkBodySize(const char *end_header)
{
	std::string body = end_header + 5;

	if (body.empty())
		return(false);
	else if (body.size() >= _bodySize)
		return(true);
	else
		return(false);
}

void 			Client::createRequest()
{
	Request request(*this);

	_request = request;
}

/*-------------OSTREAM OPERATOR------------*/

std::ostream& operator<<(std::ostream& os, const Client& client)
{
	struct sockaddr_in clientAddress = client.getClientAddress();
	std::string clientIP = inet_ntoa(clientAddress.sin_addr);
	int	clientPort = ntohs(clientAddress.sin_port);
	short clientFamily = clientAddress.sin_family;

	os << "Client fd: " << client.getSockfd() << std::endl;
	os << "Client IP: " << clientIP << std::endl;
	os << "Client Port: " << clientPort << std::endl;
	os << "Client Family: ";
	if (clientFamily == AF_INET)
		os << "AF_INET";
	else if (clientFamily == AF_INET6)
		os << "AF_INET6";
	else if (clientFamily == AF_UNSPEC)
		os << "AF_UNSPEC";
	os << "\n";
	os << "Client connected to the server fd: " << client.getServfd() << std::endl;
	return(os);
}