/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcorenti <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 00:28:50 by dcorenti          #+#    #+#             */
/*   Updated: 2023/06/19 02:36:08 by dcorenti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "../../includes/webserv.hpp"

class Client;
class Request {
	
	public:
		Request();
		Request(Client& client);
		Request(const Request& copy);
		~Request();
		Request& operator=(const Request& copy);

		/* GETTER */
		std::string 						getMethod();
		std::string 						getPath();
		std::string 						getVersion();
		std::map<std::string, std::string> 	getHeader();
		std::string 						getBody();
		std::string							getHeaderByKey(const std::string& key);
		std::string							getAllRequest();
		unsigned long						getBodySize();

		/* SETTER */
		void	setMethod(const std::string& str);
		void	setPath(const std::string& str);
		void	setVersion(const std::string& str);
		void	setHeader(const std::string& str);
		void	setBody(const std::string& str);

		/* MEMBER FUNCTIONS */
		void	parseRequest();

	private:
		/* VARIABLES */
		std::string 						_method;
		std::string 						_path;
		std::string 						_version;
		std::map<std::string, std::string> 	_header;
		std::string 						_body;
		std::string							_allRequest;
		unsigned long						_bodySize;

		void	setfirstline(std::string str);
		void	setHeaders(std::string str);
};

std::ostream& operator<<(std::ostream& os, Request& request);

#endif