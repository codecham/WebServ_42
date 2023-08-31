
#ifndef METHOD_HPP
# define METHOD_HPP

# include "../../includes/webserv.hpp"
# include "../Server/Server.hpp"
# include "../Request/Request.hpp"
# include "../Response/Response.hpp"
# include "../Server/Location.hpp"
# include "../cgi/Cgi.hpp"

# include <iostream>
# include <cstring>
# include <cctype>
# include <cstdio>
# include <string>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <unistd.h>
# include <vector>

void	Exec(Server& server, Request& request, Response& response);
void	handlePost(Server& server, Request& request, Response& response);
void	handleGet(Server& server, Request& request, Response& response);
void 	handleCgi(Server& server, Request& request, Location& location, Response& response);
void	handleDelete(Server& server, Request& request, Response& response);

#endif
