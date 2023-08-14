#include "methods.hpp"

void	Exec(Server& server, Request& request, Response& response)
{
	std::string	method = request.getMethod();

	if (method == "GET" && server.getAllowedMethods("GET"))
		handleGet(server, request, response);
	else if (method == "POST" && server.getAllowedMethods("POST"))
		handlePost(server, request, response);
	else if (method == "DELETE" && server.getAllowedMethods("DELETE"))
		std::cout << "DELETE METHOD CALLED"<< std::endl;
	else
		response.createResponse("405", server);
}
