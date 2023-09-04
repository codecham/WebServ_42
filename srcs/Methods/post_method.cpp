/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post_method.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dduvivie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 12:11:37 by dduvivie          #+#    #+#             */
/*   Updated: 2023/09/04 12:11:38 by dduvivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "methods.hpp"

std::string	get_absolute_path(Server& server, Location& location)
{
	std::string	fullPath;
	std::string	root;
	std::string directory;

	root = location.getRoot();
	if (root.empty())
		root = server.getRoot();
	directory = location.getUploadStore();
	if (directory.empty())
		return ("");
	fullPath = root + directory;
	return (fullPath);
}

void	upload_file(Server& server, Request& request, Response& response)
{
	int 		code;
	Location	location;
	std::string	fullPath;
	std::string	path = request.getPath();

	path = decodeUrl(path);
	if (server.locationExist(path))
	{
		location = server.getLocationByPath(path);
		if (!location.getAllowedMethods("POST"))
		{
			response.createResponse("405", server);
			return ;
		}
		if (!server.checkMaxBodySize(request.getBodySize()))
		{
			response.createResponse("413", server);
			return ;
		}
		fullPath = location.getUploadStore();
		if (fullPath.empty())
		{
			std::cout << RED << "Can't upload file if upload_store is not define" << RESET << std::endl;
			response.createResponse("500", server);
			return ;
		}
		if (!isDirectory(fullPath))
		{
			std::cout << RED << fullPath << " is not a directory" << std::endl;
			response.createResponse("500", server);
			return ;
		}
		code = createBinaryFileFromBody(request, fullPath);
		response.createResponse(to_string(code), server);
	}
	else
		response.createResponse("500", server);
	return ;
}

void	non_upload_file(Server& server, Request& request, Response& response)
{
	Location	location;
	std::string	path = request.getPath();

	path = decodeUrl(path);
	if (server.locationExist(path))
	{
		location = server.getLocationByPath(path);
		if (!location.getAllowedMethods("POST"))
		{
			response.createResponse("405", server);
			return ;
		}
		if (!server.checkMaxBodySize(request.getBodySize()))
		{
			response.createResponse("413", server);
			return ;
		}
		if (location.isCgiRequest(request.getPath()))
			return(handleCgi(server, request, location, response));
	}
	else
	{
		if (!server.getAllowedMethods("POST"))
		{
			response.createResponse("405", server);
			return ;
		}
		if (!server.checkMaxBodySize(request.getBodySize()))
		{
			response.createResponse("413", server);
			return ;
		}
	}
	response.createResponse("204", server);
}

void	handlePost(Server& server, Request& request, Response& response)
{
	if (request.isUpload())
		upload_file(server, request, response);
	else
	{
		non_upload_file(server, request, response);
	}
	return ;
}
