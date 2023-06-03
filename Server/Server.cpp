/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcorenti <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 13:39:18 by dcorenti          #+#    #+#             */
/*   Updated: 2023/06/03 22:51:52 by dcorenti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/*
	Constructors, Destructors, Operators
*/

Server::Server()
{
	_port = 0;
	_host = 0;
	_name = "";
	_max_body_size = 0;
	_error_pages[301] = "";
	_error_pages[302] = "";
	_error_pages[400] = "";
	_error_pages[401] = "";
	_error_pages[402] = "";
	_error_pages[403] = "";
	_error_pages[404] = "";
	_error_pages[405] = "";
	_error_pages[406] = "";
	_error_pages[500] = "";
	_error_pages[501] = "";
	_error_pages[502] = "";
	_error_pages[503] = "";
	_error_pages[505] = "";
	_error_pages[505] = "";
	_is_default = false;
	_listen_fd = 0;
	fd = 0;
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
		_is_default = copy._is_default;
		_locations = copy._locations;
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
		_is_default = copy._is_default;
		_locations = copy._locations;
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
	Log(GREEN, "PARSING", "SET HOST: " + value);
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
	Log(GREEN, "PARSING", "SET NAME: " + value);
	_name = value;
}

void	Server::setPort(std::string value)
{
	long tmp;
	char *endptr;

	isValidToken(value);
	Log(GREEN, "PARSING", "SET PORT: " + value);
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
	Log(GREEN, "PARSING", "SET MAX_BODY_SIZE: " + value);
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
	Log(GREEN, "PARSING", "SET ERROR PAGE: " + value);
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
		need to check if the file exist
	*/
	_error_pages.insert(std::make_pair((short)tmp, vec[1]));
}

void 	Server::setLocation(std::string path, Location location)
{
	Log(GREEN, "PARSING", "ADD LOCATION TO SERVER CONFIG: " + path);
	std::map<std::string, Location>::iterator it = 	_locations.find(path);
	if (it != _locations.end())
		throw std::runtime_error("Duplicated Location block found");
	_locations.insert(std::make_pair(location.getPath(), location));
}

void	Server::setDefault(bool value)
{
	if (value == true)
		_is_default = true;
	else
		_is_default = false;
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

bool 							Server::getIsDefault() const
{
	return(_is_default);
}

std::map<std::string, Location> Server::getLocation() const
{
	return(_locations);
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

// void	setupServer(void)
// {
	
	
// }

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
	os << "IsDefault: " << server.getIsDefault() << std::endl;
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