#include "methods.hpp"

/*
	Exemple for a POST method with a upload file:

	When you see that the request is for upload a file you can use the function createBinaryFile

	PARAM:
		- Request
		- A string that is the directory where the file will be store
	RETURN
		- Return the http code (int) that you can use for build response. For exemple, if the upload was sucessfull that's return 200
*/

std::string	get_absolute_path(Server& server, std::string path, std::string root)
{
	std::string	fullPath;

	if (!root.empty())
		fullPath = root + path;
	else
		fullPath = server.getRoot() + path;
	return (fullPath);
}

void	handlePost(Server& server, Request& request, Response& response)
{
	int 		code;
	Location	location;
	std::string	fullPath;
	std::string	uploadstore;
	std::string path = request.getPath();

	if (server.locationExist(path))
	{
		location = server.getLocationByPath(path);
		if (!location.getAllowedMethods("POST"))
		{
			response.createResponse("405", server);
			return ;
		}
		// -------- LOCATION ISSUE ------------
		std::cout << "Location: " << location << std::endl;
		uploadstore = location.getUploadStore();
		std::cout << "uploadstore: " << uploadstore << std::endl;
		if (uploadstore.empty())
			fullPath = get_absolute_path(server, path, location.getRoot());
		else
			fullPath = get_absolute_path(server, uploadstore, location.getRoot());
	}
	else
	{
		std::cout << "LOCATION NOT EXIST" << std::endl;
		if (!server.getAllowedMethods("POST"))
		{
			response.createResponse("405", server);
			return ;
		}
		fullPath = get_absolute_path(server, path, "");
	}

	std::cout << "fullPath: " << fullPath << std::endl;

	if (request.isUpload())
	{
		code = createBinaryFileFromBody(request, fullPath);
		std::cout << "code: " << code << std::endl;
		response.createResponse(to_string(code), server);
		return;
	}
	else
	{
		//parse_data(request.getBody());
		response.createResponse("204", server);
	}
}
