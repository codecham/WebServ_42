/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcorenti <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 23:41:00 by dcorenti          #+#    #+#             */
/*   Updated: 2023/06/23 02:34:25 by dcorenti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/webserv.hpp"

int main(int argc, char **argv)
{
	std::list<Server> server_list;
	std::string configfile;
	
	if (argc > 2)
		return(0);
	if (argc == 1)
		configfile = "config/config.conf";
	else
		configfile = argv[1];
	try 
	{
		ParserConfig config(configfile);						//Send the filename for parsing
		server_list = config.getServer();						//Get the list of server object created in ParserConfig
		ServerManager manager(server_list);
		manager.closeServers();
	}
	catch (const std::exception& e) 
	{
		Log(RED, "ERROR", e.what());
	}
}