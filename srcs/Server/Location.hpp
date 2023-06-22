/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcorenti <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 19:40:34 by dcorenti          #+#    #+#             */
/*   Updated: 2023/06/22 20:29:27 by dcorenti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <string>
#include <map>
#include <vector>
#include "../../includes/webserv.hpp"
#include <stdexcept>

/*
	path 			-> Path Location
	root			-> Directory or file whre the file should be search
	autoIndex 		-> Directory listening (on or off)
	methods			-> Methods accepted (default = all accepted)
	index			-> Default file if the answer is a directory
	cgi_path		-> Path for cgi
	cgi_ext			-> Extension's accepted for cgi
	max_body_size   -> Maximum size for the client body 
*/

class Location
{
	public:
		Location();
		Location(const Location& copy);
		~Location();
		Location& operator=(const Location& copy);

		void setPath(std::string value);
		void setRoot(std::string value);
		void setAutoIndex(std::string value);
		void setIndex(std::string value);
		void setAllowMethod(std::string value);
		void setDenyMethod(std::string value);
		void setRedirection(std::string value);
		void setCgiPath(std::string value);
		void setCgiExt(std::string value);
		void setClientBodySize(std::string value);
		void setErrorPage(std::string value);
		void setUploadStore(std::string value);

		std::string 						getPath() const;
		std::string 						getRoot() const;
		bool								getAutoIndex() const;
		std::string							getIndex() const;
		std::vector<bool> 					getMethods() const;
		std::map<std::string, std::string> 	getRedirection() const;
		std::vector<std::string> 			getCgiPath() const;
		std::vector<std::string> 			getCgiExt() const;
		unsigned long						getClientBodySize() const;
		bool								getAllowedMethods(std::string method) const;
		std::map<short, std::string>		getErrorPages() const;
		std::string 					    getErrorPageCode(short code);
		std::string 						getUploadStore() const;

	private:
		std::string							_path;
		std::string							_root;
		bool								_autoindex;
		std::string							_index;
		std::vector<bool>					_methods; // 0 -> GET | 1 -> POST | 2 -> DELETE
		std::map<std::string, std::string>	_redirection;
		std::vector<std::string>			_cgi_path;
		std::vector<std::string>			_cgi_ext;
		unsigned int 						_max_body_size;
		std::map<short, std::string>  		_error_pages;
		std::string							_upload_store;

		void 	isValidToken(std::string& token);
		void	isValidPath(std::string& path);
};

std::ostream& operator<<(std::ostream& os, const Location& location);

#endif