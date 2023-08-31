#include "methods.hpp"

void	Exec(Server& server, Request& request, Response& response)
{
	std::string	method = request.getMethod();

	if (method == "GET")
		handleGet(server, request, response);
	else if (method == "POST")
		handlePost(server, request, response);
	else if (method == "DELETE")
		std::cout << "DELETE METHOD CALLED"<< std::endl;
	else
		response.createResponse("405", server);
}