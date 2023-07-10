/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcorenti <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 00:00:25 by dcorenti          #+#    #+#             */
/*   Updated: 2023/06/14 03:19:10 by dcorenti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSERCONFIG_HPP
#define PARSERCONFIG_HPP

#include "../../includes/webserv.hpp"
#include "../Server/Server.hpp"
#include "../utils/Log.hpp"

#include <iostream>
#include <string>
#include <stdexcept>
#include <fstream>
#include <list>
#include <map>
#include <vector>

class Server;
class Location;
class ParserConfig
{
	public:
		ParserConfig(const std::string filename);
		~ParserConfig();
		
		std::list<Server> getServer() const;

	private:
		std::ifstream _file;
		std::list<Server> _serverlist;
		std::map<int, std::vector<std::string> > _extractedFile;

		ParserConfig();
		ParserConfig(const ParserConfig& copy);
		ParserConfig& operator=(const ParserConfig& copy);

		void 	ParseFile();
		bool	isEndBlock(std::vector<std::string> token);
		void	isValidLocation(std::string token);
		void	insertParamServer(Server& server, std::vector<std::string> token);
		void	insertParamLocation(Location& location, std::vector<std::string> token);
		void	CheckServer(Server& server);
};

#endif