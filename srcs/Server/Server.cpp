/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcorenti <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 13:39:18 by dcorenti          #+#    #+#             */
/*   Updated: 2023/06/27 04:22:02 by dcorenti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/*
	Constructors, Destructors, Operators
*/

Server::Server()
{
	std::string error_directory = DEFAULT_ERROR_PAGE_DIRECTORY;
	
	_port = 0;
	_host = 0;
	_name = "";
	_index = "";
	_root = "";
	_max_body_size = 0;
	_error_pages[301] = error_directory + "301.html";
	_error_pages[302] = error_directory + "302.html";
	_error_pages[400] = error_directory + "400.html";
	_error_pages[401] = error_directory + "401.html";
	_error_pages[402] = error_directory + "402.html";
	_error_pages[403] = error_directory + "403.html";
	_error_pages[404] = error_directory + "404.html";
	_error_pages[405] = error_directory + "405.html";
	_error_pages[406] = error_directory + "406.html";
	_error_pages[500] = error_directory + "500.html";
	_error_pages[501] = error_directory + "501.html";
	_error_pages[502] = error_directory + "502.html";
	_error_pages[503] = error_directory + "503.html";
	_error_pages[504] = error_directory + "504.html";
	_error_pages[505] = error_directory + "505.html";
	_methods.push_back(true); //GET
	_methods.push_back(false); //POST
	_methods.push_back(false); //DELETE
	_fd = 0;
}

Server::Server(const Server& copy)
{
	if (this != &copy)
	{
		_port = copy._port;
		_host = copy._host;
		_name = copy._name;
		_max_body_size = copy._max_body_size;
		_error_pages = copy._error_pages;
		_locations = copy._locations;
		_fd = copy._fd;
		_index = copy._index;
		_root = copy._root;
		_methods = copy._methods;
	}
}

Server::~Server()
{

}

Server&	Server::operator=(const Server& copy)
{
	if (this != &copy)
	{
		_port = copy._port;
		_host = copy._host;
		_name = copy._name;
		_max_body_size = copy._max_body_size;
		_error_pages = copy._error_pages;
		_locations = copy._locations;
		_fd = copy._fd;
		_index = copy._index;
		_root = copy._root;
		_methods = copy._methods;
	}
	return(*this);
}

/*
	Setters
*/

void	Server::setHost(std::string value)
{
	std::string address;

	isValidToken(value);
	if (value == "localhost")
		address = "127.0.0.1";
	else
		address = value;
	if (!isValidHost(address))
		throw std::runtime_error("Invalid host");
	_host = inet_addr(address.data());
}

void	Server::setName(std::string value)
{
	isValidToken(value);
	_name = value;
}

void	Server::setPort(std::string value)
{
	long tmp;
	char *endptr;

	isValidToken(value);
	tmp = strtol(value.c_str(), &endptr, 10);
	if (*endptr)
		throw std::runtime_error("Wrong character in port");
	if (tmp < std::numeric_limits<uint16_t>::min() || tmp > std::numeric_limits<uint16_t>::max())
		throw std::runtime_error("Wrong port value");
	_port = (uint16_t)tmp;
}

void	Server::setMaxBodySize(std::string value)
{
	long tmp;
	char *endptr;

	isValidToken(value);
	tmp = strtol(value.c_str(), &endptr, 10);
	if (*endptr)
		throw std::runtime_error("Wrong character in MaxBodySize");
	if (tmp < std::numeric_limits<unsigned int>::min() || tmp > std::numeric_limits<unsigned int>::max() || value.size() > 11)
		throw std::runtime_error("Wrong value for MaxBodySize");
	_max_body_size = (unsigned int)tmp;
}

void	Server::setErrorPage(std::string value)
{
	std::vector<std::string> vec;
	long tmp;
	char *endptr;

	isValidToken(value);
	if (value.empty())
		return ;
	vec = splitInVector(value, ' ');
	if (vec.size() != 2 || vec[0].size() != 3)
		throw std::runtime_error("Wrong Error_page format");
	tmp = strtol(vec[0].c_str(), &endptr, 10);
	if (*endptr)
		throw std::runtime_error("Wrong Error_page format");
	if (tmp < 301 || tmp > 505)
		throw std::runtime_error("Wrong code error. Must be between 301 and 505");
	/*
		need to check if the file exist?
	*/
	_error_pages.insert(std::make_pair((short)tmp, vec[1]));
}

void	Server::setRoot(std::string value)
{
	isValidToken(value);
	if(!(isDirectory(value)))
		throw std::runtime_error("Root path is not a directory");
	_root = value;
}

void	Server::setIndex(std::string value)
{
	isValidToken(value);
	_index = value;
}

void 	Server::setLocation(std::string path, Location location)
{
	std::map<std::string, Location>::iterator it = 	_locations.find(path);
	if (it != _locations.end())
		throw std::runtime_error("Duplicated Location block found");
	_locations.insert(std::make_pair(location.getPath(), location));
}

void 	Server::setAllowMethod(std::string value)
{
	isValidToken(value);
	if (value == "GET")
		_methods[GET] = true;
	else if (value == "POST")
		_methods[POST] = true;
	else if (value == "DELETE")
		_methods[DELETE] = true;
	else	throw std::runtime_error("Invalid Allowed Method");

}

void 	Server::setDenyMethod(std::string value)
{
	isValidToken(value);
	if (value == "GET")
		_methods[GET] = false;
	else if (value == "POST")
		_methods[POST] = false;
	else if (value == "DELETE")
		_methods[DELETE] = false;
	else
		throw std::runtime_error("Invalid Allowed Method");
}

/*
	Getters
*/

in_addr_t 						Server::getHost() const
{
	return(_host);
}

std::string 					Server::getName() const
{
	return(_name);
}

uint16_t 						Server::getPort() const
{
	return(_port);
}

unsigned int					Server::getMaxBodySize() const
{
	return(_max_body_size);
}

std::map<short, std::string>	Server::getErrorPages() const
{
	return (_error_pages);
}

std::string 					Server::getErrorPageCode(short code)
{
	return(_error_pages[code]);
}

std::map<std::string, Location> Server::getLocation() const
{
	return(_locations);
}

std::string 					Server::getRoot() const
{
	return(_root);
}

std::string						Server::getIndex() const
{
	return (_index);
}

Location 						Server::getLocationByPath(std::string path)
{
	return (_locations[path]);
}

bool							Server::locationExist(std::string path)
{
	if (_locations.find(path) != _locations.end())
		return (true);
	else
		return (false);
}

int								Server::getfd() const
{
	return(_fd);
}

bool							Server::getAllowedMethods(std::string method) const
{
	if (method == "GET")
		return(_methods[GET]);
	if (method == "POST")
		return(_methods[POST]);
	if (method == "DELETE")
		return(_methods[DELETE]);
	return (false);
}

/*
	Members Functions
*/

void	Server::isValidToken(std::string& token) const
{
	size_t pos = token.rfind(';');

	if (pos != token.size() - 1)
		throw std::runtime_error("Token is invalid");
	token.erase(pos);
	if (token.empty())
		throw std::runtime_error("Token is invalid");
}

bool 	Server::isValidHost(std::string host) const
{
	struct sockaddr_in sockaddr;
  	return (inet_pton(AF_INET, host.c_str(), &(sockaddr.sin_addr)) ? true : false);
}

void 	Server::createSocket()
{
	struct addrinfo *result;
	struct addrinfo hints;
	int ret;
	struct addrinfo *tmp;
	struct in_addr addr;
	std::string ip;
	
	addr.s_addr = _host;
	ip = inet_ntoa(addr);
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	ret = getaddrinfo(inet_ntoa(addr), to_string(_port).c_str(), &hints, &result);
	if (ret != 0)
		throw std::runtime_error(gai_strerror(ret));
	tmp = result;
	while (tmp != NULL)
	{
		_fd = socket(tmp->ai_family, tmp->ai_socktype, tmp->ai_protocol);
		if (_fd != -1)
		{
			if (bind(_fd, tmp->ai_addr, tmp->ai_addrlen) == 0)
				break ;
		}
		tmp = tmp->ai_next;
		close(_fd);
	}
	freeaddrinfo(result);
	if (tmp == NULL)
		throw std::runtime_error("Can't socket or bind host " + ip + ":" + to_string(_port));
	if (!_name.empty())
		Log(GREEN, "SERVER", "Socket for " + _name + " on " + ip + ":" + to_string(_port) + " successfully created");
	else
		Log(GREEN, "SERVER", "Socket for " + ip + ":" + to_string(_port) + " successfully created");
}

void	Server::closeSocket()
{
	close(_fd);
	Log(YELLOW, "SERVER", "Socket closed");
}

/*
	ostream operator
*/

std::ostream& operator<<(std::ostream& os, const Server& server)
{
	struct in_addr addr;
	std::map<std::string, Location> location = server.getLocation();
	std::map<std::string, Location>::iterator it = location.begin();

	addr.s_addr = server.getHost();

	os << "Server Config: \n" << std::endl;
	os << "Host: " << inet_ntoa(addr) << std::endl;
	os << "Port: " << server.getPort() << std::endl;
	os << "Name: " << server.getName() << std::endl;
	os << "MaxBodySize: " << server.getMaxBodySize() << std::endl;
	os << "Index: " << server.getIndex() << std::endl;
	os << "Root: " << server.getRoot() << std::endl;
	os << "GET: " << server.getAllowedMethods("GET") << std::endl;
	os << "POST: " << server.getAllowedMethods("POST") << std::endl;
	os << "DELETE: " << server.getAllowedMethods("DELETE") << std::endl;
	if (!(location.empty()))
	{
		while(it != location.end())
		{
			os << it->second << std::endl;
			it++;
		}
	}
	return(os);
}