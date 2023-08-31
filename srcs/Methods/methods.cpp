#include "methods.hpp"

void	Exec(Server& server, Request& request, Response& response)
{
	std::string	method = request.getMethod();

	if (method == "GET")
		handleGet(server, request, response);
	else if (method == "POST" || method == "PUT")
		handlePost(server, request, response);
	else if (method == "DELETE")
		handleDelete(server, request, response);
	else
		response.createResponse("405", server);
}
