
#ifndef METHOD_HPP
# define METHOD_HPP

# include "../../includes/webserv.hpp"
# include "../Server/Server.hpp"
# include "../Request/Request.hpp"
# include "../Response/Response.hpp"
# include "../Server/Location.hpp"

# include <iostream>
# include <cstring>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <unistd.h>

void	Exec(Server& server, Request& request, Response& response);

#endif