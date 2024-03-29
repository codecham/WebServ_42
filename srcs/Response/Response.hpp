/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcorenti <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 18:35:06 by dcorenti          #+#    #+#             */
/*   Updated: 2023/08/19 02:26:17 by dcorenti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "../../includes/webserv.hpp"
#include "../Server/Server.hpp"

#include <iostream>
#include <string>
#include <fstream>
#include <dirent.h>
#include <sys/stat.h>

class Server;
class Response
{
	public:
		Response();
		Response(const Response& copy);
		~Response();
		Response& operator=(const Response& copy);

		/*-----------SETTERS-----------*/
		void setVersion(const std::string& str);
		void setCode(const std::string& str);
		void setMessage(const std::string& str);
		void setHeader(const std::string& key, const std::string& value);
		void setBody(const std::string& str);
		void setPage(const std::string& file);
		void setConnexionClose();

		/*------------GETTER-----------*/
		std::string getVersion() const;
		std::string getCode() const;
		std::string getMessage() const;
		std::map<std::string, std::string> getHeader() const;
		std::string getHeaderByKey(std::string key);
		std::string getBody() const;
		std::string getResponse() const;
		std::string getResponseNoBody() const;
		bool		getConnexionClose();

		void 		buildResponse();
		void		createResponse(const std::string& code, const std::string& file, Server& server);
		void		createResponse(const std::string& response);
		void		createResponse(const std::string& code, Server& server);
		void		createResponseAutoIndex(const std::string& directory, Server& server, std::string& path);

	private:
		std::string	_version;
		std::string _code;
		std::string _message;
		std::map<std::string, std::string> _header;
		std::string _body;
		std::string _response;
		std::map<std::string, std::string> _mimeTypes;
		std::map<int, std::string>		   _errors_pages;
		Server	_server;
		bool	_closeConnexion;

		void		insert_mimes_types(std::string key, std::string value);
		void		create_mimes_types();
		std::string createline(std::string line);
		void		addFileToBody(const std::string& fileName);
		void		setMimeType(const std::string& fileName);
		void		cleanErrorsPage();
		void		setErrorPage(const std::string& code);
		int			setAutoIndex(const std::string& directory, std::string& path);
		std::string setHtmlAutoIndexPage(std::multimap<int, std::string>& contentMap, const std::string& path);
};

std::ostream& operator<<(std::ostream& os, const Response& response);

#endif