/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcorenti <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 00:28:50 by dcorenti          #+#    #+#             */
/*   Updated: 2023/06/14 03:19:15 by dcorenti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/webserv.hpp"
#ifndef REQUEST_HPP
#define REQUEST_HPP

class Request {
	
	public:
		Request();
		Request(std::string& str);
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

		/* SETTER */
		void	setMethod(const std::string& str);
		void	setPath(const std::string& str);
		void	setVersion(const std::string& str);
		void	setHeader(const std::string& str);
		void	setBody(const std::string& str);

	private:
		/* VARIABLES */
		std::string 						_method;
		std::string 						_path;
		std::string 						_version;
		std::map<std::string, std::string> 	_header;
		std::string 						_body;
};

std::ostream& operator<<(std::ostream& os, Request& request);

#endif