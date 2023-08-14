#include "methods.hpp"

void	create_request_responce(Server& server, bool isredirection, std::string fullPath, Response& response)
{
	if (fileExist(fullPath))
	{
		if (readRights(fullPath))
		{
			if (isredirection)
				response.createResponse("301", fullPath, server);
			else
				response.createResponse("200", fullPath, server);
			return ;
		}
		response.createResponse("403", server);
		return ;
	}
	response.createResponse("404", server);
	return ;
}

void	handleGet(Server& server, Request& request, Response& response)
{
	Location	location;
	std::string root;
	std::string fullPath;
	std::string redirection;
	std::string path = request.getPath();
	bool		isredirection = false;

	if (server.locationExist(path))
	{
		location = server.getLocationByPath(path);
		redirection = location.getRedirection(path);
		if (!redirection.empty()) // if redirection exist
		{
			path = redirection;
			isredirection = true;
		}
		root = location.getRoot();
		if (!root.empty()) // if root is set
			fullPath = root + path;
		else
			fullPath = server.getRoot() + path;
		if (isDirectory(fullPath)) // if path is directory
		{
			if (location.getAutoIndex() == true)
				response.createResponseAutoIndex(fullPath, server, path);
			if (!location.getIndex().empty())  // index is set?
			{
				fullPath += location.getIndex();
				create_request_responce(server, isredirection, fullPath, response);
				return ;
			}
			fullPath += "/index.html";
			create_request_responce(server, isredirection, fullPath, response);
			return ;
		}
		else
		{
			create_request_responce(server, isredirection, fullPath, response);
			return ;
		}
	}
	else // Location doesn't exist
	{
		fullPath = server.getRoot() + path;
		if (isDirectory(fullPath))
		{
			fullPath += "index.html";
			create_request_responce(server, isredirection, fullPath, response);
		}
		else
			create_request_responce(server, isredirection, fullPath, response);
		return ;
	}
}
