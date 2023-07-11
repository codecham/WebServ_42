#include "Method.hpp"

/*
	Hello :) 
	I hope you have slept well! Take a good coffee and here we go! 

	I modified the response object. You can use it in 2 ways:

		- When you want send a file: 

			resonse.createResponse([STATUS_CODE], [FILE], server);

		- When you don't want send a file (for error code for exemple): 

			response.createResposne([STATUS_CODE], server);

*/

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
	response.createResponse("200", fullPath, server);
	// response.createResponse("404", server); 				<-- exemple for a error response

}