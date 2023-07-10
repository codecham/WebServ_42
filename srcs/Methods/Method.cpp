#include "Method.hpp"

void	Exec(Server& server, Request& request, Response& response)
{
    // GET METHOD 

    // 
	std::string root = server.getRoot();
	std::string path = request.getPath();
	std::string fullPath;


	fullPath += root;
	fullPath += path;
	if (isDirectory(path))
		fullPath += "index.html";
	response.createResponse("200", fullPath);

}