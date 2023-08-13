#include "Method.hpp"

/*

*/

void	Exec(Server& server, Request& request, Response& response)
{
	Location	location;
	std::string root;
	std::string fullPath;
	std::string redirection;
	std::string path = request.getPath();
	bool		isredirection = false;

	/*
		Exemple for a POST method with a upload file:

			When you see that the request is for upload a file you can use the function createBinaryFile
			
			PARAM:
					- Request 
					- A string that is the directory where the file will be store
			
			RETURN
					- Return the http code (int) that you can use for build response. For exemple, if the upload was sucessfull that's return 200
	*/
	// std::cout << "Server: " << server << std::endl;
	if (request.getMethod() == "POST" && request.isUpload())
	{
		int code;

		code = createBinaryFileFromBody(request, "/Users/corentin/Desktop/19/webserv_2/");
		response.createResponse(to_string(code), server);
		return;
	}

	if (server.getAllowedMethods("GET"))
	{
		if (server.locationExist(path))
		{
			location = server.getLocationByPath(path);
			if (!location.getAllowedMethods("GET"))
				response.createResponse("403", server);
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
				// autoindex is set ?
					// yes ==> build autoindex response (FINISH)
				if (location.getAutoIndex())
				{
					response.createResponseAutoIndex(fullPath, server, path);
					return ;
				}
				if (!location.getIndex().empty())  // index is set?
				{
					fullPath += location.getIndex();
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
				fullPath += "/index.html";
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
			else
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
		}
		else // Location doesn't exist
		{
			fullPath = server.getRoot() + path;
			if (isDirectory(fullPath))
			{
				fullPath += "index.html";
				if (fileExist(fullPath))
				{
					if (readRights(fullPath))
					{
						response.createResponse("200", fullPath, server);
						return ;
					}
					response.createResponse("403", server);
					return ;
				}
				response.createResponse("404", server);
				return ;
			}
			else
			{
				if (fileExist(fullPath))
				{
					if (readRights(fullPath))
					{
						response.createResponse("200", fullPath, server);
						return ;
					}
					response.createResponse("403", server);
					return ;
				}
				response.createResponse("404", server);
				return ;
			}
		}
	}
	response.createResponse("403", server);
}