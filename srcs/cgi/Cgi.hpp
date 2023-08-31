/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcorenti <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 21:10:59 by dcorenti          #+#    #+#             */
/*   Updated: 2023/08/24 19:29:34 by dcorenti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP

#include "../../includes/webserv.hpp"
#include <string>
#include <map>
#include <unistd.h>
#include <stdio.h>

# define CGI_BUFSIZE 65536

class Request;
class Server;
class Location;
class Log;
class Cgi
{
	public:
		Cgi();
		Cgi(const Cgi& copy);
		~Cgi();
		Cgi& operator=(const Cgi& copy);

		void 		initCgi(Server& server, Request& request, Location& location);
		void		printEnv() const;
		void		printCgi() const;
		std::string	executeCgi();
		int			getStatusCode() const;

	private:
		std::map<std::string, std::string>	_envMap;
		std::string							_scriptName;
		std::string							_params;
		std::string							_cgiDirectory;
		std::string							_interpreter;
		int									_statusCode;
		std::string							_fullPath;
		std::string							_serverName;
		std::string							_port;
		std::string							_method;
		std::string							_content_lenght;
		std::string							_content_type;
		std::string							_http_cookie;
		std::string							_path;


		void		extractScriptName(std::string path);
		void		extractParam(Request& request);
		void		setDirectory(Location& location);
		void	 	setInterpret(Location& location);
		void		setFullPath();
		void		setStatusCode(int code, const std::string& message);
		void		setServerData(Server& server);
		void		setEnvMap();
		char		**getEnvAsCstrArray() const;
		void		setPath(std::string path);
		std::string 	decode(std::string str);
};


#endif