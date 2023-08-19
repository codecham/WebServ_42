#include "methods.hpp"

std::string	get_value(std::string str)
{
	std::size_t		i = 0;
	const char*		startPos;
	const char*		endPos;

	startPos = str.c_str();
	while (i < str.length() && std::isspace(str[i]))
	{
		i++;
		startPos++;
	}
	endPos = startPos;
	while (i < str.length() && str[i] != '\n')
	{
		i++;
		endPos++;
	}
	std::string	value(startPos, endPos);
	return (value);
}


void	print_value(std::string content, std::string form_data)
{
	std::size_t	found;
	std::string	value;

	found = content.find(form_data);
	if (found!=std::string::npos)
	{
		value = content.substr(found + form_data.size());
		value = get_value(value);
		if (form_data == "name=\"firstName\"")
			std::cout << "firstName = " << value << std::endl;
		else if (form_data == "name=\"lastName\"")
			std::cout << "lastName = " << value << std::endl;
		else if (form_data == "name=\"message\"")
			std::cout << "message = " << value << std::endl;
	}
}

std::string	get_boundary(std::string body)
{
	const char*		startPos;
	const char*		endPos;

	startPos = body.c_str();
	endPos = startPos;
	while (*endPos != '\0' && *endPos != '\n')
		++endPos;
	std::string	boundary(startPos, endPos);
	return (boundary);
}

void	print_data(Server& server, Request& request)
{
	int							i = 0;
	std::string					body;
	std::string					boundary;
	std::vector<std::string>	splited_body;

	(void)server;
	body = request.getBody();
	if (body.empty())
		return ;
	boundary = get_boundary(body);
	if (boundary.empty())
		return ;
	splited_body = splitInVectorByString(body, boundary);
	for (std::vector<std::string>::iterator it = splited_body.begin(); it != splited_body.end(); ++it)
	{
		if (!(*it).empty())
		{
			if (i == 0)
				print_value(*it, "name=\"firstName\"");
			else if (i == 1)
				print_value(*it, "name=\"lastName\"");
			else
				print_value(*it, "name=\"message\"");
			i++;
		}
	}
}

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

void	handlePost(Server& server, Request& request, Response& response)
{
	if (request.isUpload())
	{
		upload_file(server, request, response);
		return ;
	}
	else
	{
		print_data(server, request);
		response.createResponse("204", server);
	}
}
