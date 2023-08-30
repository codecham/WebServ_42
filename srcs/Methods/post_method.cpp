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

	if (server.locationExist(path))
	{
		location = server.getLocationByPath(path);
		if (!location.getAllowedMethods("POST"))
		{
			response.createResponse("405", server);
			return ;
		}
		fullPath = get_absolute_path(server, location);
		if (fullPath.empty() || !isDirectory(fullPath))
		{
			response.createResponse("500", server);
			return ;
		}
		code = createBinaryFileFromBody(request, fullPath);
		std::cout << "code: " << code << std::endl;
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

	if (server.locationExist(path))
	{
		location = server.getLocationByPath(path);
		if (!location.getAllowedMethods("POST"))
		{
			response.createResponse("405", server);
			return ;
		}
	}
	else
	{
		if (!server.getAllowedMethods("POST"))
		{
			response.createResponse("405", server);
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
