/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcorenti <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 19:30:09 by dcorenti          #+#    #+#             */
/*   Updated: 2023/07/11 01:53:05 by dcorenti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location()
{
	_path = "";
	_root = "";
	_autoindex = "";
	_index = "";
	_autoindex = false;
	_methods.push_back(true); //GET
	_methods.push_back(false); //POST
	_methods.push_back(false); //DELETE
	_max_body_size = 0;
}

Location::Location(const Location& copy)
{
	if (this != &copy)
	{
		_path = copy._path;
		_root = copy._root;
		_autoindex = copy._autoindex;
		_index = copy._index;
		_methods = copy._methods;
		_redirection = copy._redirection;
		_cgi_ext = copy._cgi_ext;
		_cgi_path = copy._cgi_path;
		_max_body_size = copy._max_body_size;
	}
}

Location::~Location()
{

}

Location&	Location::operator=(const Location& copy)
{
	if (this != &copy)
	{
		_path = copy._path;
		_root = copy._root;
		_autoindex = copy._autoindex;
		_index = copy._index;
		_methods = copy._methods;
		_redirection = copy._redirection;
		_cgi_ext = copy._cgi_ext;
		_cgi_path = copy._cgi_path;
		_max_body_size = copy._max_body_size;
	}
	return(*this);
}

void	Location::setPath(std::string value)
{
	std::vector<std::string> vec = splitInVector(value, ' ');
	if (vec[1] != "{")
		throw std::runtime_error("Invalid Location syntax");
	isValidPath(vec[0]);
	_path = vec[0];
}

void	Location::setRoot(std::string value)
{
	isValidToken(value);
	// if(!(isDirectory(value)))
	// 	throw std::runtime_error("Root path is not a directory");
	_root = value;
}

void	Location::setAutoIndex(std::string value)
{
	isValidToken(value);
	if (value != "on" && value != "off")
		throw std::runtime_error("Wrong auto_index value");
	if (value == "on")
		_autoindex = true;
	if (value == "off")
		_autoindex = false;
}

void	Location::setIndex(std::string value)
{
	isValidToken(value);
	_index = value;
}

void	Location::setAllowMethod(std::string value)
{
	isValidToken(value);
	if (value == "GET")
		_methods[GET] = true;
	else if (value == "POST")
		_methods[POST] = true;
	else if (value == "DELETE")
		_methods[DELETE] = true;
	else
	{
		std::cout << value << " --> ";
		throw std::runtime_error("Invalid Allowed Method");
	}
}

void	Location::setDenyMethod(std::string value)
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

void	Location::setRedirection(std::string value)
{
	std::vector<std::string> vec;
	std::map<std::string, std::string>::iterator it;

	isValidToken(value);
	vec = splitInVector(value, ' ');
	if (vec.size() != 2)
		throw std::runtime_error("Invalid number of value for return");
	it = _redirection.find(vec[0]);
	if (it != _redirection.end())
		throw std::runtime_error("Multiple redirections for same url find");
	_redirection.insert(std::make_pair(vec[0], vec[1]));
}

void	Location::setCgiPath(std::string value)
{
	isValidToken(value);
	isValidPath(value);
	_cgi_path.push_back(value);
}

void	Location::setCgiExt(std::string value)
{
	isValidToken(value);
	if (value != ".php")
		throw std::runtime_error("Invalid cgi_ext. Only accepted .php");
	_cgi_ext.push_back(value);
}

void	Location::setClientBodySize(std::string value)
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

void 	Location::setUploadStore(std::string value)
{
	isValidToken(value);
	_upload_store = value;
}

std::string 						Location::getPath() const
{
	return (_path);
}

std::string 						Location::getRoot() const
{
	return (_root);
}

bool								Location::getAutoIndex() const
{
	return (_autoindex);
}

std::string							Location::getIndex() const
{
	return (_index);
}

std::vector<bool> 					Location::getMethods() const
{
	return (_methods);
}

std::map<std::string, std::string> 	Location::getRedirectionMap() const
{
	return (_redirection);
}

std::string  						Location::getRedirection(std::string str)
{
	return(_redirection[str]);
}

std::vector<std::string> 			Location::getCgiPath() const
{
	return (_cgi_path);
}

std::vector<std::string> 			Location::getCgiExt() const
{
	return (_cgi_ext);
}

unsigned long						Location::getClientBodySize() const
{
	return (_max_body_size);
}

bool								Location::getAllowedMethods(std::string method) const
{
	if (method == "GET")
		return(_methods[GET]);
	if (method == "POST")
		return(_methods[POST]);
	if (method == "DELETE")
		return(_methods[DELETE]);
	return (false);
}

std::string		Location::getUploadStore() const
{
	return(_upload_store);
}


void 	Location::isValidToken(std::string& token)
{
	size_t pos = token.rfind(';');

	if (pos != token.size() - 1)
		throw std::runtime_error("Token is invalid");
	token.erase(pos);
	if (token.empty())
		throw std::runtime_error("Token is invalid");

}

void	Location::isValidPath(std::string& str)
{
	if (str.empty())
		throw std::runtime_error("Invalid path");
	if (str[0] != '/')
		throw std::runtime_error("Invalid path");
	for (std::size_t i = 1; i < str.size(); i++)
	{
		if (!(isalnum(str[i]) || str[i] == '-' || str[i] == '_' || str[i] == '.' || str[i] == '/'))
			throw std::runtime_error("Invald token in path");
		if (str[i] == '/' && str[i - 1] == '/')
			throw std::runtime_error("Invalid path format");
	}
}

bool 	Location::redirectionExist(std::string path)
{
	if (_redirection.find(path) != _redirection.end())
		return(true);
	else
		return(false);
}

std::ostream& operator<<(std::ostream& os, const Location& location)
{
	std::map<std::string, std::string>	redirection = location.getRedirectionMap();
	std::map<std::string, std::string>::iterator it = redirection.begin();
	std::vector<std::string> cgi_path = location.getCgiPath();
	std::vector<std::string> cgi_ext = location.getCgiExt();
	unsigned int i = 0;

	os << "\nlocation " << location.getPath() << " :" << std::endl;
	os << "root: " << location.getRoot() << std::endl;
	os << "index: " << location.getIndex() << std::endl;
	os << "auto_index: " << location.getAutoIndex() << std::endl;
	os << "GET: " << location.getAllowedMethods("GET") << std::endl;
	os << "POST: " << location.getAllowedMethods("POST") << std::endl;
	os << "DELETE: " << location.getAllowedMethods("DELETE") << std::endl;
	os << "upload_store: " << location.getUploadStore() << std::endl;
	if (!(redirection.empty()))
	{
		os << "redirection: " << std::endl;
		while (it != redirection.end())
		{
			os << "\t" << it->first << " --> " << it->second << std::endl;
			it++;
		}
	}
	if (!(cgi_path.empty()))
	{
		os << "Cgi path: " << std::endl;
		while (i < cgi_path.size())
		{
			os << "\t" << cgi_path[i] << std::endl;
			i++;
		}
	}
	if (!(cgi_ext.empty()))
	{
		i = 0;
		os << "Cgi ext: " << std::endl;
		while (i < cgi_ext.size())
		{
			os << "\t" << cgi_ext[i] << std::endl;
			i++;
		}
	}
	return(os);
}
