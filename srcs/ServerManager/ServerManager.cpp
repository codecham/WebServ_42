/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcorenti <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 15:41:29 by dcorenti          #+#    #+#             */
/*   Updated: 2023/08/31 20:50:41 by dcorenti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerManager.hpp"

/*
	Constructors, Assignement Operator, Destructors
*/

ServerManager::ServerManager()
{

}

ServerManager::ServerManager(std::list<Server> server_list)
{
	_nbServer = 0;
	_nbClient = 0;
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
	
	while (it != _server_list.end())
	{
		if (it->getHost() < 0)
			throw std::runtime_error("host not define or not good for a server");
		if (it->getPort() <= 0)
			throw std::runtime_error("Port not define or not good for a server");
		if (it->getRoot().empty())
			throw std::runtime_error("Root for server not define. It's mandatory");
		it++;
	}
	it = _server_list.begin();
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
			it->createSocket();
			setOptionSocket(it->getfd());
			listen(it->getfd(), MAX_CLIENTS * 10);
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


void 	ServerManager::sendMaxClientPage(Client& client)
{
	Server serv = getServerForRequest(client);
	std::string errorPage = serv.getErrorPageCode(503);
	int bytes;

	bytes = send(client.getSockfd(), errorPage.c_str(), errorPage.size(), 0);
	if (bytes < 0)
		Log(RED, "INFO", "Failed to send on socket: " + to_string(client.getSockfd()));
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
	Add serv socket fds to pollfds vector
*/

void	ServerManager::addServToPoll()
{
	std::map<int, std::vector<Server> >::iterator it = _serv_fds.begin();

	while (it != _serv_fds.end())
	{
		struct pollfd tmp;
		memset(&tmp, 0, sizeof(tmp));
		tmp.fd = it->first;
		tmp.events = POLLIN | POLLOUT | POLLERR | POLLHUP;
		tmp.revents = 0;
		_pollFds.push_back(tmp);
		it++;
	}	
}

/*
	Add a client fd to pollfds vector
*/

void	ServerManager::addClientToPoll(Client& client)
{
	struct pollfd tmp;
	memset(&tmp, 0, sizeof(tmp));
	tmp.fd = client.getSockfd();
	tmp.events = POLLIN | POLLOUT | POLLERR | POLLHUP | POLLPRI | POLLNVAL;
	tmp.revents = 0;
	_pollFds.push_back(tmp);
	_nbClient++;
}

/*
	Close a client socket and earse him of pollfds vector
*/

void	ServerManager::closeClientConnection(Client& client)
{
	std::vector<struct pollfd>::iterator it = _pollFds.begin();

	while (it != _pollFds.end())
	{
		if (it->fd == client.getSockfd())
		{
			_pollFds.erase(it);
			break ;
		}
		it++;
	}
	Log(ORANGE, "INFO", "Client connexion closed on socket: " + to_string(client.getSockfd()));
	close(client.getSockfd());
	_clients_list.erase(client.getSockfd());
	_nbClient--;
}

/*
	find a client in the client list by the fd 
*/

Client& ServerManager::getClientByFd(int fd)
{
	std::map<int, Client>::iterator it = _clients_list.begin();

	while(it != _clients_list.end() && it->first != fd)
		it++;
	return(it->second);
}


/*
	Read the client's data send
*/

bool	ServerManager::readClientData(Client& client)
{
	char buf[1024];
	int bytes;

	bzero(&buf, 1024);
	bytes = recv(client.getSockfd(), &buf, 1024, 0);
	if (bytes < 0)
	{
		Log(RED, "INFO", "Failed to recv");
		return(false);
	}
	if (bytes == 0)
		return(true);
	buf[bytes] = '\0';
	client.setDataRecv(buf, bytes);
	client.setLastTime();
	client.checkEndRequest();
	return(true);
}


/*
	This function run an infinite loop and wait event on our servers sockets

	While a clients try to connect to a socket, we catch the event and exec all function for manage them
*/

void	ServerManager::runServers()
{
	int poll_result;
	unsigned int i = 0;
	_nbClient = 0;

	addServToPoll();
	_nbServer = _pollFds.size();
	Log(BLUE, "INFO", "Waiting client connexion");
	while(1)
	{
		i = 0;
		poll_result = poll(&_pollFds[0], _pollFds.size(), -1);
		if (poll_result < 0)
			throw	std::runtime_error("Poll() Failed !!");
		/*
			Check the servers events
		*/
		while (i < _pollFds.size())
		{
			if (i < _nbServer)
			{
				/*
					Check POLLIN event on a server Socket.
					If POLLIN is detected, create a new Client object and add 
					it to pollFds and in the list of client
				*/
				if (_pollFds[i].revents & POLLIN)
				{
					int newClientSocket = 0;
					Client newClient;
					struct sockaddr_in clientAddress;
					socklen_t clientAddressLength = sizeof(clientAddress);
					memset(&clientAddress, 0, sizeof(clientAddress));
					newClientSocket = accept(_pollFds[i].fd, (struct sockaddr *)&clientAddress, &clientAddressLength);
					Log(GREEN, "INFO", "New client detected");
					if (newClientSocket >= 0)
					{
						setOptionSocket(newClientSocket);
						newClient.setSockfd(newClientSocket);
						newClient.setClientAddress(clientAddress);
						newClient.setClientAdressLenght(clientAddressLength);
						newClient.setServfd(_pollFds[i].fd);
						_clients_list.insert(std::make_pair(newClientSocket, newClient));
						addClientToPoll(newClient);
						if (_nbClient > MAX_CLIENTS)
						{
							Log(BLUE, "INFO", "Maximum number of client reached. The new client will be disconnected: " + to_string(_nbClient));
							sendMaxClientPage(newClient);
							closeClientConnection(newClient);	
						}
					}
					else
						Log(RED, "INFO", "Accept Failed");
				}
			}
			/*
				Check the clients events.
			*/
			else
			{
				/*
					Get the client object in the client list.
				*/
				Client& client = getClientByFd(_pollFds[i].fd);


				/*
					Check if the timout is reached.
				*/
				if (client.isTimeOut())
				{
					send(client.getSockfd(), timeOutPage().c_str(), timeOutPage().size(), 0);
					Log(ORANGE, "INFO", "Client timeout");
					client.resetClient();
					closeClientConnection(client);
				}
				else
				{
					/*
						If POLLIN is detected, read the data in the socket,
						parse it and add it to client
					*/
					if (_pollFds[i].revents & POLLIN)
					{
						if (!readClientData(client))
							closeClientConnection(client);
					}
					/*
						If the Client socket is ready to POLLOUT and if the HTTP 
						request is complete, create a HTTP response and send it
					*/
					if (_pollFds[i].revents & POLLOUT && client.getEndRequest())
					{
						if (!client.getResponseBuild())
							buildResponse(client);
						client.sendResonse();
						
						/*
							If all response is send, we reset the client object.
							If the connexion type is "close", we close the socket 
							and delete de client of the pollFds and the client list
						*/
						if (client.getAllResponseSend())
						{
							if (client.isCloseConnexion())
							{
								Log(PURPLE, "INFO", "Client is a closed connexion");
								closeClientConnection(client);
							}
							else
								client.resetClient();
						}
					}
					/*
						Check the POLLERR and POLLHUP event, and close the 
						client connexion if it's detected
					*/
					if (_pollFds[i].revents & POLLERR)
					{
						Log(RED, "INFO", "POLLERR detected");
						closeClientConnection(client);
					}
					if (_pollFds[i].revents & POLLHUP)
					{
						Log(YELLOW, "INFO", "Client close connexion on fd: " + to_string(client.getSockfd()));
						closeClientConnection(client);
					}
				}
			}
			i++;
		}
	}
	Log(YELLOW, "INFO", "Closing servers....");
	closeServers();
	// system("leaks webserv");
}

Server	ServerManager::getServerForRequest(Client& client)
{
	Request request = client.getRequest();
	std::string host = request.getHeaderByKey("Host");
	std::vector<Server> servers = _serv_fds[client.getServfd()];
	size_t nFind;

	nFind = host.find(":");
	if (nFind != std::string::npos)
		host = host.substr(0, nFind);
	for (unsigned int i = 0 ; i < servers.size() ; i++)
	{
		if (servers[i].getName() == host)
			return(servers[i]);
	}
	return(servers[0]);
}

void	ServerManager::buildResponse(Client& client)
{
	client.createRequest();
	Request request = client.getRequest();
	Server	server = getServerForRequest(client);
	Response response;

	Log(BRIGHT_BLUE, "INFO", "HTTP request received on socket " + to_string(client.getSockfd()) + ":");
	std::cout << PURPLE << request << RESET << std::endl;
	Exec(server, request, response);
	client.setResponse(response.getResponse());
	if (response.getConnexionClose())
		client.setCloseConnexion();
	Log(ORANGE, "INFO", "Response:");
	std::cout << CYAN << response << RESET << std::endl;
	std::cout << "-------------------------------------------------\n" << std::endl;
}