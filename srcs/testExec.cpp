/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testExec.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcorenti <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 23:51:00 by dcorenti          #+#    #+#             */
/*   Updated: 2023/06/27 03:35:48 by dcorenti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/webserv.hpp"

void	testExec(Server& server, Request& request, Response& response)
{
	std::string root = server.getRoot();
	std::string path = request.getPath();
	std::string fullPath;


	fullPath += root;
	fullPath += path;
	if (isDirectory(path))
		fullPath += "index.html";
	response.createResponse("200", fullPath);
}
