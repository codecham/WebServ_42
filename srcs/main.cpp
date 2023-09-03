/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcorenti <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 23:41:00 by dcorenti          #+#    #+#             */
/*   Updated: 2023/09/03 23:05:28 by dcorenti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/webserv.hpp"
#include "../srcs/ParserConfig/ParserConfig.hpp"
#include "../srcs/ServerManager/ServerManager.hpp"
#include "../srcs/utils/Log.hpp"
#include <signal.h>

ServerManager* g_manager = 0;


void signalHandler(int signal)
{
	(void)signal;
	if (g_manager)
	{
		std::cout << BLUE << "\nSignal ctrl+c received. Servers will be closed" << RESET << std::endl;
		delete g_manager;
		g_manager = 0;
	}
	// system("leaks webserv");
	exit(EXIT_SUCCESS);
}

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
		// ServerManager manager(server_list);
		g_manager = new ServerManager(server_list);
		std::signal(SIGINT, (void (*)(int))signalHandler);
		g_manager->runServers();
	}
	catch (const std::exception& e) 
	{
		Log(RED, "ERROR", e.what());
	}
}