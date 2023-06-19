/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcorenti <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 15:41:29 by dcorenti          #+#    #+#             */
/*   Updated: 2023/06/19 03:28:18 by dcorenti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/webserv.hpp"


/*
	Constructors, Assignement Operator, Destructors
*/

ServerManager::ServerManager()
{

}

ServerManager::ServerManager(std::list<Server> server_list)
{
	_server_list = server_list;
	checkServers();
	createSockets();
	setServfds();
	signal(SIGPIPE, sigPipeHandler);
	runServers();
}

ServerManager::ServerManager(const ServerManager& copy)
{
	_server_list = copy._server_list;
	_serv_fds = copy._serv_fds;
	_servSocket = copy._servSocket;
}

ServerManager::~ServerManager()
{

}

ServerManager&	ServerManager::operator=(const ServerManager& copy)
{
	if (this != &copy)
	{
		_server_list = copy._server_list;
		_serv_fds = copy._serv_fds;
		_servSocket = copy._servSocket;
	}
	return(*this);
}



/*
	This function check if there is no duplicate server.

	We can have different servers on the same host:port but only if there server_names are differents.
*/

void	ServerManager::checkServers()
{
	std::list<Server>::iterator it = _server_list.begin();
	std::list<Server>::iterator it_next = it;
	
	it_next++;
	while (it != _server_list.end())
	{
		while (it_next != _server_list.end())
		{
			if (it->getHost() == it_next->getHost() && it->getPort() == it_next->getPort() && it->getName() == it_next->getName())
				throw std::runtime_error("Duplicate host:port + server_name found");
			it_next++;
		}
		it++;
		it_next = it;
		it_next++;
		if (it_next == _server_list.end())
			break ;
	}
}



/*
	This function will create and bind socket for every server in our serverlist

	Before socket and bind, it check if the host:port is already done or not. 
*/

void	ServerManager::createSockets()
{
	std::list<Server>::iterator it = _server_list.begin();

	while (it != _server_list.end())
	{
		if (checkduplicate(it))
		{
			it->setDefault(true);
			it->createSocket();
			fcntl(it->getfd(), F_SETFL, O_NONBLOCK);
			listen(it->getfd(), MAX_CLIENTS);
		}
		it++;
	}
}



/*
	This function check if the host:port of the server already exist on the previous serverlist 

	Return true if there is no other same host:port found or false if it's found 
*/

bool 	ServerManager::checkduplicate(std::list<Server>::iterator it)
{
	std::list<Server>::iterator previous = _server_list.begin();

	if (it == _server_list.begin())
		return(true);
	while (previous != it)
	{
		if (previous->getHost() == it->getHost() && previous->getPort() == it->getPort())
			return (false);
		previous++;
	}
	return(true);
}



/*
	This function will close each open server sockets
*/

void	ServerManager::closeServers()
{
	std::list<Server>::iterator it = _server_list.begin();

	while (it != _server_list.end())
	{
		if (checkduplicate(it))
			it->closeSocket();
		it++;
	}
}



/*
	Function will create a map with fds sockets 
	and all servers listen on this same fds
*/

void	ServerManager::setServfds()
{
	std::list<Server>::iterator it = _server_list.begin();
	std::list<Server>::iterator it_next = _server_list.begin();
	std::vector<Server> tmp;

	while (it != _server_list.end())
	{
		it_next = it;
		it_next++;
		if (it->getfd() != 0)
		{
			tmp.push_back(*it);
			while (it_next != _server_list.end())
			{
				if (it->getHost() == it_next->getHost() && it->getPort() == it_next->getPort())
					tmp.push_back(*it_next);
				it_next++;
			}
			_serv_fds.insert(std::make_pair(it->getfd(), tmp));
		}
		it++;
		tmp.clear();
	}
}



/*
	This function run an infinite loop and wait event on our servers sockets

	While a clients try to connect to a socket, we catch the event and exec all function for manage them

*/

void	ServerManager::runServers()
{
	struct pollfd poll_fds[MAX_CLIENTS + _serv_fds.size()];
	std::map<int, std::vector<Server> >::iterator it = _serv_fds.begin();
	int clientSockets[MAX_CLIENTS];
	unsigned int i = 0;
	int poll_result;

	memset(&poll_fds, 0, sizeof(poll_fds));
	memset(&clientSockets, 0, sizeof(clientSockets));
	while (it != _serv_fds.end())
	{
		_servSocket.push_back(it->first);
		poll_fds[i].fd = it->first;
		poll_fds[i].events = POLLIN;
		it++;
		i++;
	}
	Log(BLUE, "INFO", "Waiting clients connexions...");
	while(1)
	{
		i = 0;
		poll_result = poll(poll_fds, MAX_CLIENTS + _serv_fds.size(), 0);
		if (poll_result > 0)
		{
			checkpollServ(poll_fds);
			checkpollClient(poll_fds);
			// checkClientRequest(poll_fds);
		}
		checkClientsTimeOut();
		clearClients(poll_fds);
	}
}



/*
	Check event on the server socket.
	If a new client is detected, add him in the client list
*/

void	ServerManager::checkpollServ(struct pollfd *poll_fds)
{
	unsigned int i = 0;

	while (i < _servSocket.size())
	{
		if (poll_fds[i].revents & POLLIN)
		{
			/*
				New client on the server
			*/
			Client new_client;
			struct sockaddr_in clientAddress;
			socklen_t clientAddressLength = sizeof(clientAddress);
			memset(&clientAddress, 0, sizeof(clientAddress));
			int newClientSocket = accept(_servSocket[i], (struct sockaddr *)&clientAddress, &clientAddressLength);
			fcntl(newClientSocket, F_SETFL, O_NONBLOCK);
			new_client.setSockfd(newClientSocket);
			new_client.setClientAddress(clientAddress);
			new_client.setClientAdressLenght(clientAddressLength);
			new_client.setServfd(poll_fds[i].fd);
			addClientPoll(poll_fds, new_client);
		}
		i++;
	}
}



/*
	Add the client to the pollsfd for check event.
	If the max client is reached, the client is rejected and his socket is close. 
*/

void	ServerManager::addClientPoll(struct pollfd *poll_fds, Client& client)
{
	unsigned int i = 0;
	
	while (i < MAX_CLIENTS + _servSocket.size())
	{
		if (poll_fds[i].fd == 0)
		{
			// std::cout << "Add the client in this index: " << i << std::endl;
			poll_fds[i].fd = client.getSockfd();
			poll_fds[i].events = (POLLIN | POLLHUP | POLLERR);
			break ;
		}
		i++;
	}
	if (i >= MAX_CLIENTS + _servSocket.size())
	{
		Log(YELLOW, "Maximum number of Client reached. Client rejected");
		close(client.getSockfd());
		return ;
	}
	_clients_list.push_back(client);
	Log(GREEN, "INFO", "New client accepted: ");
	std::cout << client << std::endl;
}



/*
	This function search the server corresponding to the fd in the server list
*/

Server& ServerManager::getServByFd(int fd)
{
	std::list<Server>::iterator it = _server_list.begin();

	while (it != _server_list.end() && it->getfd() != fd)
		it++;
	return(*it);
}



/*
	Check event on the client socket.
	If client event is detected, add the data recv to the client objet
*/

void	ServerManager::checkpollClient(struct pollfd *poll_fds)
{
	unsigned int i = 0;

	while (i < _serv_fds.size())
		i++;
	while (i < MAX_CLIENTS + _serv_fds.size())
	{
		if (poll_fds[i].revents & (POLLIN | POLL_HUP))
		{
			char buffer[8192];
			int bytesRead = recv(poll_fds[i].fd, buffer, sizeof(buffer), 0);
			if (bytesRead <= 0)
			{
				/*
					Client's deconnexion or error
				*/
				Client& client = findClientByfd(poll_fds[i].fd);
				client.setInactive();
			}
			else
			{
				buffer[bytesRead] = '\0';
				addDataToClient(poll_fds, i, buffer, bytesRead);
				checkRequest(poll_fds[i].fd);
			}
		}
		i++;
	}
}



/*
	Add the data recv in the client object
*/

void	ServerManager::addDataToClient(struct pollfd *poll_fds, int i, char *buffer, int bytesRead)
{
	Client& client = findClientByfd(poll_fds[i].fd);

	buffer[bytesRead] = '\0';
	client.setDataRecv(buffer);
}



/*
	Return the client object find in the list of client
*/

Client&	ServerManager::findClientByfd(int fd)
{
	unsigned int i = 0;

	while(i < _clients_list.size() && _clients_list[i].getSockfd() != fd)
		i++;
	return(_clients_list[i]);
}


/*
	In building
*/

void	ServerManager::checkClientRequest(struct pollfd *poll_fds)
{
	unsigned int i = 0;

	while (i < _serv_fds.size())
		i++;
	while (i < MAX_CLIENTS + _serv_fds.size() && poll_fds[i].fd != 0)
	{
		Client client = findClientByfd(poll_fds[i].fd);
		std::cout << CYAN << "Client fd: " << client.getSockfd() << RESET << std::endl;
		std::cout << CYAN << "Data in client:\n" << client.getDataRecv() << RESET << std::endl;
		i++;
	}
}


/*
	This function check the time of clients are connected.
	If CONNEXION_TIMEOUT is reached for one or many clients, there status being inactive and they will be deconnected in the runfunction
*/

void	ServerManager::checkClientsTimeOut()
{
	unsigned int i = 0;

	while (i < _clients_list.size())
	{
		if (time(NULL) - _clients_list[i].getLastTime() > CONNECTION_TIMEOUT)
		{
			Log(YELLOW, "INFO", "Client on connected to fd " + std::to_string(_clients_list[i].getSockfd()) + " Timeout. Connection will be closed.");
			send(_clients_list[i].getSockfd(), timeOutPage().c_str(), timeOutPage().size(), 0);
			_clients_list[i].setInactive();
		}
		i++;
	}
}



/*
	This function check all client's status. If they are inactives, that close there connexions
*/

void	ServerManager::clearClients(struct pollfd *poll_fds)
{
	unsigned int i = 0;

	while (i < _serv_fds.size())
		i++;
	while (i < MAX_CLIENTS + _serv_fds.size())
	{
		if(poll_fds[i].fd != 0)
		{
			Client& client = findClientByfd(poll_fds[i].fd);
			if (!client.getActive())
			{
				eraseClientsListByfd(poll_fds[i].fd);
				close(poll_fds[i].fd);
				Log(YELLOW, "INFO", "Connexion close for client on the socket " + std::to_string(poll_fds[i].fd));
				poll_fds[i].fd = 0;
			}
		}
		i++;
	}
}



/*
	Erase a client in the client list searching by his fd
*/

void	ServerManager::eraseClientsListByfd(int fd)
{
	unsigned int i = 0;

	while (i < _clients_list.size())
	{
		if (_clients_list[i].getSockfd() == fd)
		{
			_clients_list.erase(_clients_list.begin() + i);
			return ;
		}
		i++;
	}
}



/*
	Check if the Request is a HTTP.
	After that check if all request is receive or not
*/

void	ServerManager::checkRequest(int fd)
{
	Client& client = findClientByfd(fd);

	if (!client.isHTTPrequest())
	{
		/*
			Code for not HTTP REQUEST
		*/
		return ;
	}
	if (client.allDataReceive())
	{
		client.createRequest();
		execRequest(client);
	}
}

/*
	Search the server who's need to response if we have some server on the same host:port
	
	If no one corresponding to the server name, first server is used by default

*/

Server ServerManager::getServerForRequest(Client& client)
{
	Request request = client.getRequest();
	std::string host = request.getHeaderByKey("Host");
	std::vector<Server> servers = _serv_fds[client.getServfd()];

	for (unsigned int i = 0 ; i < servers.size() ; i++)
	{
		if (servers[i].getName() == host)
			return(servers[i]);
	}
	return(servers[0]);
}

/*
	Print the ServFds. Using for debug
*/

void	ServerManager::printServfds()
{
	std::map<int, std::vector<Server> >::iterator it = _serv_fds.begin();
	unsigned int i;

	std::cout << BLUE << "Sockets and Servers who's use each socket" << RESET << std::endl;
	while (it != _serv_fds.end())
	{
		i = 0;
		std::vector<Server> tmp = it->second;
		std::cout << GREEN << "Socket fd: " << it->first << RESET << std::endl;
		while (i < tmp.size())
		{
			std::cout << "host:port: " << tmp[i].getHost() << ":" << tmp[i].getPort() << " | server_name: " << tmp[i].getName() << std::endl;
			i++;
		}
		std::cout << std::endl;
		it++;
	}
}




/*------------------------------------------------------------------------*/

void	ServerManager::execRequest(Client& client)
{
	Request request = client.getRequest();
	Server server = getServerForRequest(client);

	Log(BLUE, "INFO", "HTTP request received:");
	std::cout << request << std::endl;

	/*
	
		The execution of the request start here! :)
		So your object or your code will be called here.

		You will receive:

		- Request: The object who's contains the parsed request

		- Server: The object who's contains the Server Config

		--> The goal now is to execute the request and create a HTTP Response to send to the client


		For the moment, with the code bellow, that just send a little HTTP response define in /utils/utils.cpp.
		When you want to test you can replace "htmltestpage" with your result for sending to the client 

	*/

	send(client.getSockfd(), htmltestpage().c_str(), htmltestpage().size(), 0);
	client.setInactive();
}