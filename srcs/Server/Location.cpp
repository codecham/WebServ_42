/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcorenti <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 19:30:09 by dcorenti          #+#    #+#             */
/*   Updated: 2023/06/20 16:45:00 by dcorenti         ###   ########.fr       */
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
		_error_pages = copy._error_pages;
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
		_error_pages = copy._error_pages;
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

void	Location::setErrorPage(std::string value)
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
	_error_pages[tmp] = vec[1];
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

std::map<std::string, std::string> 	Location::getRedirection() const
{
	return (_redirection);
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

std::map<short, std::string>		Location::getErrorPages() const
{
	return(_error_pages);
}

std::string 					    Location::getErrorPageCode(short code)
{
	return(_error_pages[code]);
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

std::ostream& operator<<(std::ostream& os, const Location& location)
{
	std::map<std::string, std::string>	redirection = location.getRedirection();
	std::map<short, std::string>  		error_pages = location.getErrorPages(); 
	std::map<std::string, std::string>::iterator it = redirection.begin();
	std::map<short, std::string>::iterator it_errpage = error_pages.begin();
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
	if (!(redirection.empty()))
	{
		os << "redirection: " << std::endl;
		while (it != redirection.end())
		{
			os << "\t" << it->first << " --> " << it->second << std::endl;
			it++;
		}
	}
	if (!(error_pages.empty()))
	{
		while (it_errpage != error_pages.end())
		{
			if (!it_errpage->second.empty())
				os << "\t" << it_errpage->first << " --> " << it_errpage->second << std::endl;
			it_errpage++;
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
