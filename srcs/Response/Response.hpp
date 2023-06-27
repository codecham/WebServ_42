/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcorenti <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 18:35:06 by dcorenti          #+#    #+#             */
/*   Updated: 2023/06/27 00:03:48 by dcorenti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include <string>
#include "../../includes/webserv.hpp"

class Response
{
	public:
		Response();
		Response(const Response& copy);
		~Response();
		Response& operator=(const Response& copy);

		/*-----------SETTERS-----------*/
		void setVersion(const std::string str);
		void setCode(const std::string str);
		void setMessage(const std::string str);
		void setHeader(const std::string key, const std::string value);
		void setBody(const std::string str);
		void setPage(const std::string file);

		/*------------GETTER-----------*/
		std::string getVersion() const;
		std::string getCode() const;
		std::string getMessage() const;
		std::map<std::string, std::string> getHeader() const;
		std::string getHeaderByKey(std::string key);
		std::string getBody() const;
		std::string getResponse() const;

		void 		buildResponse();
		void		createResponse(std::string code, std::string file);

	private:
		std::string	_version;
		std::string _code;
		std::string _message;
		std::map<std::string, std::string> _header;
		std::string _body;
		std::string _response;
		std::map<std::string, std::string> _mimeTypes;

		void	insert_mimes_types(std::string key, std::string value);
		void	create_mimes_types();
		std::string createline(std::string line);
};

std::ostream& operator<<(std::ostream& os, const Response& response);

#endif