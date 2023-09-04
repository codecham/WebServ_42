/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete_method.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dduvivie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 12:11:03 by dduvivie          #+#    #+#             */
/*   Updated: 2023/09/04 12:11:10 by dduvivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "methods.hpp"

void	handleDelete(Server& server, Request& request, Response& response)
{
	Location	location;
	std::string root;
	std::string fullPath;
	std::string	path = request.getPath();

	path = decodeUrl(path);
	if (server.locationExist(path))
	{
		location = server.getLocationByPath(path);
		if (!location.getAllowedMethods("DELETE"))
		{
			response.createResponse("405", server);
			return ;
		}
		root = location.getRoot();
		if (!root.empty())
			fullPath = root + path;
		else
			fullPath = server.getRoot() + path;
		if (!fileExist(fullPath))
		{
			response.createResponse("404", server);
			return ;
		}
		if (remove(fullPath.c_str()) == 0)
		{
			response.createResponse("204", server);
			return ;
		}
		response.createResponse("403", server);
	}
	else
	{
		if (!server.getAllowedMethods("DELETE"))
		{
			response.createResponse("405", server);
			return ;
		}
		fullPath = server.getRoot() + path;
		if (!fileExist(fullPath))
		{
			response.createResponse("404", server);
			return ;
		}
		if (remove(fullPath.c_str()) == 0)
		{
			response.createResponse("204", server);
			return ;
		}
		response.createResponse("403", server);
	}
	return ;
}
