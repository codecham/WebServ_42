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

void	handlePost(Server& server, Request& request, Response& response)
{
	int 		code;
	Location	location;
	std::string	directory;
	std::string path = request.getPath();

	location = server.getLocationByPath(path);
	directory = location.getUploadStore();
	if (request.isUpload())
	{
		if (directory.empty())
			directory = path;
		code = createBinaryFileFromBody(request, directory);
		response.createResponse(to_string(code), server);
		return;
	}
	else
	{
		std::cout << "RECEIVED DATA FROM CLIENT" << std::endl;
		// work in progress
	}
}
