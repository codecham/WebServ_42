/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testExec.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcorenti <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 23:51:00 by dcorenti          #+#    #+#             */
/*   Updated: 2023/07/10 13:52:53 by dcorenti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/webserv.hpp"

/*

	Here's a basic exemple of exec a request!

		1) Get the root of the server with server.getRoot();
		2) Get the path of the request with request.getPath();
		3) Build the full path of the search file -> root + path
		4) Create a response with:
			- The status code
			- The full path after build
*/


void	testExec(Server& server, Request& request, Response& response)
{
	std::string root = server.getRoot();
	std::string path = request.getPath();
	std::string fullPath;

	fullPath += root;
	fullPath += path;

	// root -> var/www/website/ 
	// file ->  /images/logo.png

	//fullpath -> /var/www/website/images/logo.png

	if (isDirectory(path))
		fullPath += "index.html";

	response.createResponse("200", fullPath);
}
