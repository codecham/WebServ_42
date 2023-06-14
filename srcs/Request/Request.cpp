/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcorenti <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 00:53:15 by dcorenti          #+#    #+#             */
/*   Updated: 2023/06/14 02:22:57 by dcorenti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request()
{

}

Request::Request(std::string& str)
{
	(void)str;
}

Request::Request(const Request& copy)
{
	_method = copy._method;
	_path = copy._path;
	_version = copy._version;
	_header = copy._header;
	_body = copy._body;
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
	}
	return(*this);
}

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

std::ostream& operator<<(std::ostream& os, Request& request)
{
	std::map<std::string, std::string> header = request.getHeader();
	std::map<std::string, std::string>::iterator it = header.begin();

	os << request.getMethod() << " ";
	os << request.getPath() << " ";
	os << request.getVersion() << std::endl;
	while (it != header.end())
	{
		os << it->first << ": " << it->second << std::endl;
		it++;
	}
	os << "\n";
	os << request.getBody() << std::endl;
	return(os);
}
