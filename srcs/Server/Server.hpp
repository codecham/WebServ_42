/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcorenti <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 01:26:04 by dcorenti          #+#    #+#             */
/*   Updated: 2023/06/14 04:51:47 by dcorenti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "../../includes/webserv.hpp"
#include <iostream>
#include <string>
#include <map>
#include <list>
#include <cstdint>
#include <stdexcept>
#include <arpa/inet.h>
#include <limits>
#include "Location.hpp"

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
		void setDefault(bool value);
		void setRoot(std::string value);
		void setIndex(std::string value);
		void setLocation(std::string path, Location location);

		/*-----------GETTERS------------*/
		in_addr_t 						getHost() const;
		std::string 					getName() const;
		uint16_t 						getPort() const;
		unsigned int					getMaxBodySize() const;
		std::map<short, std::string>	getErrorPages() const;
		std::string 					getErrorPageCode(short code);
		bool 							getIsDefault() const;
		std::string						getRoot() const;
		std::string						getIndex() const;
		std::map<std::string, Location> getLocation() const;
		Location						getLocationByPath(std::string path);

		/*-----------MEMBERS FUNCTIONS------------*/

		bool locationExist(std::string path);
		void createSocket();
		void closeSocket();
		
	private:
		in_addr_t 						_host;
		std::string 					_name;
		uint16_t						_port;
		unsigned int					_max_body_size;
		std::map<short, std::string>  	_error_pages;
		bool							_is_default;
		std::string						_root;
		std::string						_index;
		std::map<std::string, Location> _locations;
		int 							_listen_fd;
		int								_fd;
		
		void isValidToken(std::string& token) const;
		bool isValidHost(std::string host) const;
};

std::ostream& operator<<(std::ostream& os, const Server& server);

#endif