/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcorenti <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 00:53:15 by dcorenti          #+#    #+#             */
/*   Updated: 2023/06/30 03:31:49 by dcorenti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Client/Client.hpp"
#include "Request.hpp"

Request::Request()
{

}

Request::Request(Client& client)
{
	_allRequest = client.getDataRecv();
	_bodySize = client.getBodySize();
	parseRequest();
}

Request::Request(const Request& copy)
{
	_method = copy._method;
	_path = copy._path;
	_version = copy._version;
	_header = copy._header;
	_body = copy._body;
	_allRequest = copy._allRequest;
	_bodySize = copy._bodySize;
}

Request::~Request()
{

}

Request&	Request::operator=(const Request& copy)
{
	if (this != &copy)
	{
		_method = copy._method;
		_path = copy._path;
		_version = copy._version;
		_header = copy._header;
		_body = copy._body;
		_allRequest = copy._allRequest;
		_bodySize = copy._bodySize;
	}
	return(*this);
}


/*-----------GETTERS------------*/

std::string 						Request::getMethod()
{
	return(_method);
}

std::string 						Request::getPath()
{
	return(_path);
}

std::string 						Request::getVersion()
{
	return(_version);
}

std::map<std::string, std::string> 	Request::getHeader()
{
	return(_header);
}

std::string 						Request::getBody()
{
	return(_body);
}

std::string							Request::getHeaderByKey(const std::string& key)
{
	if (_header.find(key) != _header.end())
		return (_header[key]);
	else
		return ("");
}

std::string							Request::getAllRequest()
{
	return(_allRequest);
}

unsigned long						Request::getBodySize()
{
	return(_bodySize);
}

/*-----------SETTERS------------*/

void	Request::setMethod(const std::string& str)
{
	_method = str;
}

void	Request::setPath(const std::string& str)
{
	_path = str;
}

void	Request::setVersion(const std::string& str)
{
	_version = str;
}

void	Request::setHeader(const std::string& str)
{
	std::string s = str;
	std::vector<std::string> vec = splitInVector(s, ':');
	strTrimedWhiteSpace(vec[0]);
	strTrimedWhiteSpace(vec[1]);
	_header.insert(std::make_pair(vec[0], vec[1]));
}

void	Request::setBody(const std::string& str)
{
	_body = str;
}

/*--------MEMBER FUNCTION--------*/

void	Request::parseRequest()
{
	std::string request = _allRequest.substr(0, _allRequest.find("\r\n\r\n"));
	std::vector<std::string> vec = splitInVectorByString(request, "\r\n");

	if (_bodySize != 0)
		_body = _allRequest.substr(_allRequest.find("\r\n\r\n"), (_allRequest.size() - request.size()));
	setfirstline(vec[0]);
	for (unsigned int i = 1 ; i < vec.size() ; i++)
		setHeader(vec[i]);
}

void	Request::setfirstline(std::string str)
{
	std::vector<std::string> vec = splitInVector(str, ' ');
	_method = vec[0];
	_path = vec[1];
	_version = vec[2];
}

void	Request::setHeaders(std::string str)
{
	std::vector<std::string> vec = split_key_value_by_c(str, ':');
	
	if (!vec[0].empty())
		strTrimedWhiteSpace(vec[0]);
	if (!vec[1].empty())
		strTrimedWhiteSpace(vec[1]);
	_header.insert(std::make_pair(vec[0], vec[1]));
}


/*-------------OSTREAM OPERATOR------------*/

std::ostream& operator<<(std::ostream& os, Request& request)
{
	// std::map<std::string, std::string> header = request.getHeader();
	// std::map<std::string, std::string>::iterator it = header.begin();

	os << request.getMethod() << " ";
	os << request.getPath() << " ";
	os << request.getVersion() << std::endl;
	// while (it != header.end())
	// {
	// 	os << it->first << ": " << it->second << std::endl;
	// 	it++;
	// }
	// os << "\n";
	// os << request.getBody() << std::endl;
	return(os);
}
