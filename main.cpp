/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcorenti <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 23:41:00 by dcorenti          #+#    #+#             */
/*   Updated: 2023/06/03 23:00:36 by dcorenti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/webserv.hpp"

/*
	This just test function who's print all the server informations getted after parsing  
*/

void print_server_list(std::list<Server>& server_list)
{
	std::list<Server>::iterator it = server_list.begin();
	
	std::cout << YELLOW << "-------------------------------------" << std::endl;
	Log(GREEN, "print servers config");	
	while(it != server_list.end())
	{
		std::cout << *it << std::endl;
		it++;
	}
}

int main(int argc, char **argv)
{
	std::list<Server> server_list;
	
	if (argc != 2)
		return(0);
	try 
	{
		Log(GREEN, "main", "Launch Parsing");				//Log() just print message in a format
		ParserConfig config(argv[1]);						//Send the filename for parsing
		Log(GREEN, "main", "Parsing complete");
		server_list = config.getServer();					//Get the list of server object created in ParserConfig		
		print_server_list(server_list);
	}
	catch (const std::exception& e) 
	{
		Log(RED, "ERROR", e.what());
	}
}