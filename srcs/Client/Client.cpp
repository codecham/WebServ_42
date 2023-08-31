/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcorenti <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 02:35:26 by dcorenti          #+#    #+#             */
/*   Updated: 2023/08/29 03:27:00 by dcorenti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client()
{
	memset(&_clientAddress, 0, sizeof(_clientAddress));
	_clientAddressLenght = sizeof(_clientAddressLenght);
	_lastTime = time(NULL);
	_isChunk = false;
	_hasBody = false;
	_bodySize = 0;
	_initialRead = true;
	_connexion = KEEP_ALIVE;
	_endRequest = false;
	_timeout = false;
	_done = false;
	_responseBuild = false;
}

Client::Client(const Client& copy)
{
	_sockfd = copy._sockfd;
	_servfd = copy._servfd;
	_clientAddress = copy._clientAddress;
	_clientAddressLenght = copy._clientAddressLenght;
	_lastTime = copy._lastTime;
	_tmp = copy._tmp;
	_reqHeader = copy._reqHeader;
	_reqBody = copy._reqBody;
	_isChunk = copy._isChunk;
	_hasBody = copy._hasBody;
	_bodySize = copy._bodySize;
	_initialRead = copy._initialRead;
	_connexion = copy._connexion;
	_endRequest = copy._endRequest;
	_timeout = copy._timeout;
	_request = copy._request;
	_response = copy._response;
	_done = copy._done;
	_responseBuild = copy._responseBuild;
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
		_lastTime = copy._lastTime;
		_tmp = copy._tmp;
		_reqHeader = copy._reqHeader;
		_reqBody = copy._reqBody;
		_isChunk = copy._isChunk;
		_hasBody = copy._hasBody;
		_bodySize = copy._bodySize;
		_initialRead = copy._initialRead;
		_connexion = copy._connexion;
		_endRequest = copy._endRequest;
		_timeout = copy._timeout;
		_request = copy._request;
		_response = copy._response;
		_done = copy._done;
		_responseBuild = copy._responseBuild;
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

void	Client::setLastTime()
{
	_lastTime = time(NULL);
}

void	Client::setResponse(std::string response)
{
	_response = response;
	_responseBuild = true;
}

void	Client::setCloseConnexion()
{
	_connexion = CLOSE;
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

time_t			Client::getLastTime() const
{
	return(_lastTime);
}

bool			Client::getEndRequest() const
{
	return(_endRequest);
}

long long		Client::getBodySize() const
{
	return(_bodySize);
}

std::string		Client::getHeaderBody() const
{
	return(_reqHeader + _reqBody);
}

Request		Client::getRequest() const
{
	return(_request);
}

std::string		Client::getHeader() const
{
	return(_reqHeader);
}

std::string 	Client::getBody() const
{
	return(_reqBody);
}

bool		Client::getResponseBuild() const
{
	return(_responseBuild);
}

bool		Client::getAllResponseSend() const
{
	return(_done);
}

/*--------MEMBER FUNCTION--------*/


void	Client::setDataRecv(char *data, int bytes)
{
	if (_initialRead)
	{
		_tmp += data;
		_initialRead = false;
		separateHeaderBody();
		if (!_initialRead)
		{
			checkBodyOrChunk();
			checkConnexionType();
		}
	}
	else
	{
		_reqBody.append(data, bytes);
	}
}

bool	Client::isCloseConnexion()
{
	if (_connexion == CLOSE)
		return(true);
	return(false);
}


void			Client::resetClient()
{
	_tmp.clear();
	_isChunk = false;
	_hasBody = false;
	_bodySize = 0;
	_initialRead = true;
	_connexion = KEEP_ALIVE;
	_endRequest = false;
	_reqBody.clear();
	_reqHeader.clear();
	_response.clear();
	_done = false;
	_responseBuild = false;
}

void			Client::separateHeaderBody()
{
	size_t nFind;

	nFind = _tmp.find("\r\n\r\n");
	if (nFind != std::string::npos)
	{
		_reqHeader = _tmp.substr(0, nFind + 4);

		if (nFind + 4 < _tmp.length())
			_reqBody = _tmp.substr(nFind + 4, _tmp.length());

		_tmp.clear();
	}
	else
		_initialRead = true;
}

void		Client::checkBodyOrChunk()
{
	size_t	nFind;
	std::string tmp;
	std::stringstream ss;
	
	nFind = _reqHeader.find("Transfer-Encoding: chunked");
	if (nFind != std::string::npos)
	{
		_hasBody = true;
		_isChunk = true;
	}
	else
	{
		_isChunk = false;
		nFind = _reqHeader.find("Content-Length: ");
		if (nFind != std::string::npos)
		{
			_hasBody = true;
			tmp = _reqHeader.substr(nFind + 16, _reqHeader.length() - (nFind + 16));
			ss << tmp;
			ss >> _bodySize;
		}
	}
}

void		Client::checkConnexionType()
{
	size_t nFind;

	nFind = _reqHeader.find("Connection: close");
	if (nFind != std::string::npos)
	{
		Log(GREEN, "INFO", "Connexion close found");
		_connexion = CLOSE;
	}
}

void		Client::checkEndRequest()
{
	if (_isChunk && _reqBody.find("0\r\n") != std::string::npos)
	{
		unchunkBody();
		_endRequest = true;
	}
	else if (!_isChunk && _hasBody && (long long)_reqBody.length() >= _bodySize)
		_endRequest = true;
	else if (!_initialRead && !_hasBody)
		_endRequest = true;
}

void		Client::unchunkBody()
{
	std::vector<std::string> split;
	std::vector<std::string>::iterator it;
	unsigned long size;
	std::stringstream unchunked_data;

	splitString(_reqBody, "\r\n", split);
	it = split.begin();
	while(it != split.end() && *it != "0")
	{
		size = strtol(it->c_str(), NULL, 16);
		it++;
		if (it == split.end() || it->length() != size)
			std::cout << RED << "Wrong size in chunked value" << std::endl;
		unchunked_data << *it;
		it++;
	}
	_reqBody = unchunked_data.str();
	_bodySize = _reqBody.length();
}

void		Client::printRequest()
{
	std::cout << _reqHeader << std::endl;
	std::cout <<  _reqBody  << std::endl;
}

void		Client::printResponse()
{
	std::cout << _response << std::endl;
}

bool		Client::isTimeOut()
{
	if (time(NULL) - _lastTime > CONNECTION_TIMEOUT)
		return(true);
	return(false);
}

void		Client::createRequest()
{
	Request request(*this);
	
	_request = request;
}

void		Client::reformResponse(int send)
{
	_response = _response.substr(send, _response.length() - send);
	if (_response.empty())
		_done = true;
}

int		Client::sendResonse()
{
	int bytesToSend;
	int bytesSend;
	
	if (_response.length() > MAXSEND)
		bytesToSend = MAXSEND;
	else
		bytesToSend = _response.length();
	bytesSend = send(_sockfd, _response.c_str(), bytesToSend, 0);
	// Log(BLUE, "INFO", "Send on socket: " + to_string(_sockfd));
	if (bytesSend < 0)
	{
		Log(RED, "INFO", "Error while sending response");
		_done = true;
		return(0);
	}
	else if (bytesSend == 0)
	{
		_done = true;
		return(0);
	}
	reformResponse(bytesSend);
	return(bytesSend);
}