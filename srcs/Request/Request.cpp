/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcorenti <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 00:53:15 by dcorenti          #+#    #+#             */
/*   Updated: 2023/09/03 20:15:30 by dcorenti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Client/Client.hpp"
#include "Request.hpp"

Request::Request()
{

}

Request::Request(Client& client)
{
	debugPrint("5.1.1.1");
	parseHeaders(client.getHeader());
	debugPrint("5.1.1.2");
	_bodySize = client.getBodySize();
	debugPrint("5.1.1.3");
	if (_bodySize > 0)
		_body = client.getBody();
	debugPrint("5.1.1.4");
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

std::string 						Request::getMethod() const
{
	return(_method);
}

std::string 						Request::getPath() const
{
	// std::cout << "hey the path is: " << std::endl;
	// std::cout << _path << std::endl;
	return(_path);
}

std::string 						Request::getVersion() const
{
	return(_version);
}

std::map<std::string, std::string> 	Request::getHeader() const
{
	return(_header);
}

std::string 						Request::getBody() const
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

std::string							Request::getAllRequest() const
{
	return(_allRequest);
}

long long							Request::getBodySize() const
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

void	Request::parseHeaders(std::string headers)
{
	std::vector<std::string> split;
	std::vector<std::string>::iterator it;

	splitString(headers, "\r\n", split);
	it = split.begin();
	setfirstline(*it);
	it++;
	while (it != split.end())
	{
		if (!it->empty())
			setHeaders(*it);
		it++;
	}
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


bool	Request::isUpload()
{
	if (_body.find("filename") != std::string::npos)
		return(true);
	else
		return(false);
}

/*-------------OSTREAM OPERATOR------------*/

std::ostream& operator<<(std::ostream& os, Request& request)
{
	std::map<std::string, std::string> header = request.getHeader();
	std::map<std::string, std::string>::iterator it = header.begin();
	std::string body;

	os << request.getMethod() << " ";
	os << request.getPath() << " ";
	os << request.getVersion() << std::endl;
	while (it != header.end() && PRINT_REQ_HEADER)
	{
		os << it->first << ": " << it->second << std::endl;
		it++;
	}
	if (!request.getBody().empty() && PRINT_REQ_BODY)
	{
		body = request.getBody();
		os << "\n";
		if (body.size() < 500)
			os << body << std::endl;
		else
			os << body.substr(0, 500) << "[...]" << std::endl;
	}
	return(os);
}
