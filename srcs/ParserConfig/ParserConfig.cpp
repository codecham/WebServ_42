/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcorenti <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 00:29:48 by dcorenti          #+#    #+#             */
/*   Updated: 2023/06/14 02:58:16 by dcorenti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParserConfig.hpp"

ParserConfig::ParserConfig(const std::string filename)
{
	Log(GREEN, "PARSING", "Open " + filename);
	_file.open(filename);
	if (!_file)
		throw std::runtime_error("Can't open file");
	Log(GREEN, "PARSING", "Extract " + filename);
	_extractedFile = extractFileInMap(_file);
	if (_extractedFile.empty())
		throw std::runtime_error("Config file is empty");
	Log(GREEN, "PARSING", "Parsing " + filename);
	ParseFile();
}

ParserConfig::~ParserConfig()
{

}

ParserConfig::ParserConfig()
{

}

ParserConfig::ParserConfig(const ParserConfig& copy)
{
	(void)copy;
}

ParserConfig&	ParserConfig::operator=(const ParserConfig& copy)
{
	(void)copy;
	return(*this);
}

/*
	GETTER
*/

std::list<Server>	ParserConfig::getServer() const
{
	return(_serverlist);
}

/*
	MEMBER FUNCTIONS
*/

void	ParserConfig::ParseFile()
{
	std::map<int, std::vector<std::string> >::iterator it = _extractedFile.begin();
	Server server;
	Server clean_server;
	Location location;
	Location clean_location;
	std::vector<std::string> line;
	std::string error;
	
	while (it != _extractedFile.end())
	{
		line = it->second;
		if (line[0] == "server" && line[1] == "{")
		{
			Log(GREEN, "PARSING", "New server block found");
			it++;
			line = it->second;
			while (it != _extractedFile.end() && !isEndBlock(line))
			{
				if (line[0] == "location")
				{
					Log(GREEN, "PARSING", "New loaction block found");
					try
					{
						location.setPath(line[1]);
					}
					catch(const std::exception& e) 
					{
						error = e.what();
						throw std::runtime_error(error + " on line " + std::to_string(it->first));
					}
					it++;
					line = it->second;
					while (it != _extractedFile.end() && !isEndBlock(line))
					{
						try 
						{
							insertParamLocation(location, line);
						}
						catch(const std::exception& e) 
						{
							error = e.what();
							throw std::runtime_error(error + " on line " + std::to_string(it->first));
						}
						it++;
						line = it->second;
					}
					if (it == _extractedFile.end() || !isEndBlock(line))
						throw std::runtime_error("Wrong Syntax, block not closed on line" + std::to_string(it->first));
					try 
					{
						server.setLocation(location.getPath(), location);
					}
					catch (const std::exception& e)
					{
						error = e.what();
						throw std::runtime_error(error + " on line " + std::to_string(it->first));
					}
					location = clean_location;
				}
				else
				{
					try 
					{
						insertParamServer(server, line);
					}
					catch (const std::exception& e) 
					{
						error = e.what();
						throw std::runtime_error(error + " on line " + std::to_string(it->first));
					}
				}
				it++;
				line = it->second;
			}
			if (it == _extractedFile.end() || !isEndBlock(line))
				throw std::runtime_error("Wrong Syntax, block not closed");
			if (_serverlist.size() == 0)
				server.setDefault(true);
			_serverlist.push_back(server);
			server = clean_server;
			it++;
		}
		else
			throw std::runtime_error("Wrong Line " + std::to_string(it->first) + " --> " + line[0] + " " + line[1]);
	}
}

bool	ParserConfig::isEndBlock(std::vector<std::string> token)
{
	if (token[0] == "}" || token[1].empty())
		return(true);
	return(false);
}

void	ParserConfig::CheckServer(Server& server)
{
	if (server.getHost() == 0)
		server.setHost("localhost");
}


void	ParserConfig::insertParamServer(Server& server, std::vector<std::string> token)
{
	std::string key = token[0];
	std::string value = token[1];
	
	if (key == "#")
		return ;
	if (value.empty())
		throw("Invalid line");
	if (key == "listen")
		server.setPort(value);
	else if (key == "server_name")
		server.setName(value);
	else if (key == "host")
		server.setHost(value);
	else if (key == "error_page")
		server.setErrorPage(value);
	else if (key == "client_max_body_size")
		server.setMaxBodySize(value);
	else if (key == "error_page")
		server.setErrorPage(value);
	else if (key == "root")
		server.setRoot(value);
	else if (key == "index")
		server.setIndex(value);
	else
		throw std::runtime_error("Invalid line");
}

void	ParserConfig::insertParamLocation(Location& location, std::vector<std::string> token)
{
	std::string key = token[0];
	std::string value = token[1];

	if (key == "#")
		return ;
	if (value.empty())
		throw("Invalid line");
	else if (key == "root")
		location.setRoot(value);
	else if (key == "index")
		location.setIndex(value);
	else if (key == "autoindex")
		location.setAutoIndex(value);
	else if (key == "allow")
		location.setAllowMethod(value);
	else if (key == "deny")
		location.setDenyMethod(value);
	else if (key == "client_max_body_size")
		location.setClientBodySize(value);
	else if (key == "cgi_path")
		location.setCgiPath(value);
	else if (key == "cgi_ext")
		location.setCgiExt(value);
	else if (key == "return")
		location.setRedirection(value);
	else if (key == "error_page")
		location.setErrorPage(value);
	else
		throw std::runtime_error("Invalid line");
}

void	ParserConfig::isValidLocation(std::string token)
{
	std::vector<std::string> _vec;

	_vec = splitInVector(token, ' ');
	if (_vec.size() != 2 || _vec[0][0] != '/' || _vec[1] != "{")
		throw std::runtime_error("invalid location syntax");
}