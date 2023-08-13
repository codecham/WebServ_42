/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcorenti <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 01:26:04 by dcorenti          #+#    #+#             */
/*   Updated: 2023/08/13 17:04:06 by dcorenti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "../../includes/webserv.hpp"
#include "Location.hpp"

#include <iostream>
#include <string>
#include <map>
#include <list>
#include <stdexcept>
#include <arpa/inet.h>
#include <cstdlib>
#include <limits>
#include <fcntl.h>

/*
	Port		->		Listening port of this server. Mandatory.
	Host		->		Listening address or 127.0.0.1 by default
	Name		->		Specify server_name, need to be added into /etc/hosts to work
	ErrorPages	->		Specify a file to send for a specific code
	MaxBodySize ->		max request body size in bytes
	Root		->		root folder of site directory, (full relative path mandatory ?)
	Index		->		Main page 
*/

class Location;
class Server
{
	public:
		Server();
		Server(const Server& copy);
		~Server();
		Server& operator=(const Server& copy);

		/*-----------SETTERS------------*/
		void setHost(std::string value);
		void setName(std::string value);
		void setPort(std::string value);
		void setMaxBodySize(std::string value);
		void setErrorPage(std::string value);
		void setRoot(std::string value);
		void setIndex(std::string value);
		void setLocation(std::string path, Location location);
		void setAllowMethod(std::string value);
		void setDenyMethod(std::string value);

		/*-----------GETTERS------------*/
		in_addr_t 						getHost() const;
		std::string 					getName() const;
		uint16_t 						getPort() const;
		unsigned int					getMaxBodySize() const;
		std::map<int, std::string>		getErrorPages() const;
		std::string 					getErrorPageCode(short code);
		bool 							getIsDefault() const;
		std::string						getRoot() const;
		std::string						getIndex() const;
		std::map<std::string, Location> getLocation() const;
		Location						getLocationByPath(std::string path);
		int								getfd() const;
		bool							getAllowedMethods(std::string method) const;

		/*-----------MEMBERS FUNCTIONS------------*/

		bool locationExist(std::string path);
		void createSocket();
		void closeSocket();
		void setupErrorPages();
		
	private:
		in_addr_t 						_host;
		std::string 					_name;
		uint16_t						_port;
		unsigned int					_max_body_size;
		std::map<int, std::string>  	_error_pages;
		std::string						_root;
		std::string						_index;
		std::map<std::string, Location> _locations;
		int								_fd;
		std::vector<bool>				_methods; // 0 -> GET | 1 -> POST | 2 -> DELETE
		
		void isValidToken(std::string& token) const;
		bool isValidHost(std::string host) const;
};

std::ostream& operator<<(std::ostream& os, const Server& server);

#endif