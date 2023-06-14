/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcorenti <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 23:41:00 by dcorenti          #+#    #+#             */
/*   Updated: 2023/06/14 04:50:58 by dcorenti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/webserv.hpp"

/*
	You can work hear

	The idea is to take server object and request object for execute the request and build an HTTP response that i can send to the client

	Server:
		The object server is build from the first server block in config/config.conf
		You can modify the config file for test all you need

	Request:
		For the moment you can create manualy a request with setters

		--> setMethod();
		--> setPath();
		--> setHeader();
		--> setBody();

	If you need some help about some stuff that i do you can tell me of course
	
	The Request class is very basic with no verifications, it's just for used manualy for the moment

	If you want modification or some stuff from my part that can help you for work, tell me :)
*/


void testExec(Server server)
{
	Request request;

	/* EXEMPLE */
	request.setMethod("GET");
	request.setPath("/");
	request.setVersion("HTTP/1.1");
	request.setHeader("Host: developer.mozilla.org");
	request.setHeader("Accept-Language: fr");
	std::cout << server << std::endl;
	std::cout << request << std::endl;
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
		Server serv = *server_list.begin();
		testExec(serv);
	}
	catch (const std::exception& e) 
	{
		Log(RED, "ERROR", e.what());
	}
}

// int main()
// {
// 	Request request;

// 	request.setMethod("GET");
// 	request.setPath("/");
// 	request.setVersion("HTTP/1.1");
// 	request.setHeader("Host: developer.mozilla.org");
// 	request.setHeader("Accept-Language: fr");
// 	std::cout << request << std::endl;
// }