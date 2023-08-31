/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cgi.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcorenti <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 23:57:11 by dcorenti          #+#    #+#             */
/*   Updated: 2023/08/24 23:31:26 by dcorenti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "methods.hpp"

void handleCgi(Server& server, Request& request, Location& location, Response& response)
{
	Cgi cgi;
	std::string newResponse;
	int status_code;

	cgi.initCgi(server, request, location);
	status_code = cgi.getStatusCode();
	if (status_code != 0)
	{
		response.createResponse(to_string(status_code), server);
		return;
	}
	// cgi.printCgi();
	newResponse = cgi.executeCgi();
	if (newResponse == "Status: 500\r\n\r\n")
		response.createResponse("500", server);
	else
	{
		response.createResponse(newResponse);
	}
	response.setConnexionClose();
}