/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcorenti <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 15:41:29 by dcorenti          #+#    #+#             */
/*   Updated: 2023/08/09 22:37:00 by dcorenti         ###   ########.fr       */
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
		if (it->getHost() <= 0)
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
	tmp.events = POLLIN | POLLOUT | POLLERR | POLLHUP;
	tmp.revents = 0;
	_pollFds.push_back(tmp);
	_nbClient++;
	// Log(BLUE, "INFO", "New client added to poll");
}

/*
	Close a client socket and earse him of pollfds vector
*/

void	ServerManager::closeClientConnection(Client& client)
{
	std::vector<struct pollfd>::iterator it = _pollFds.begin();
	unsigned int i = 0;

	while (it != _pollFds.end())
	{
		if (it->fd == client.getSockfd())
		{
			_pollFds.erase(it);
			break ;
		}
		it++;
	}
	while (i < _clients_list.size())
	{
		if (_clients_list[i].getSockfd() == client.getSockfd())
			_clients_list.erase(_clients_list.begin() + i);
		i++;
	}
	close(client.getSockfd());
	Log(ORANGE, "INFO", "Client connexion closed");
}

/*
	find a client in the client list by the fd 
*/

Client& ServerManager::getClientByFd(int fd)
{
	std::vector<Client>::iterator it = _clients_list.begin();

	while(it != _clients_list.end() && it->getSockfd() != fd)
		it++;
	return(*it);
}


/*
	Read the client's data send
*/

bool	ServerManager::readClientData(Client& client)
{
	char buf[50] = {0};
	int bytes;

	bzero(&buf, 50);
	bytes = recv(client.getSockfd(), &buf, 50, 0);

	if (bytes <= 0)
	{
		//recv error
		return(false);
	}
	buf[bytes] = '\0';
	client.setDataRecv(buf);
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

	addServToPoll();
	_nbServer = _pollFds.size();
	
	Log(BLUE, "INFO", "Waiting client connexion");
	while(1)
	{
		i = 0;
		poll_result = poll(&_pollFds[0], _pollFds.size(), -1);
		if (poll_result < 0)
			throw	std::runtime_error("Poll() Failed !!");
		while (i < _pollFds.size())
		{
			//Check Server Event
			if (i < _nbServer)
			{
				if (_pollFds[i].revents & POLLIN)
				{
					Log(GREEN, "INFO", "New connexion detected");
					int newClientSocket;
					Client newClient;
					struct sockaddr_in clientAddress;
					socklen_t clientAddressLength = sizeof(clientAddress);

					memset(&clientAddress, 0, sizeof(clientAddress));
					newClientSocket = accept(_pollFds[i].fd, (struct sockaddr *)&clientAddress, &clientAddressLength);
					setOptionSocket(newClientSocket);
					newClient.setSockfd(newClientSocket);
					newClient.setClientAddress(clientAddress);
					newClient.setClientAdressLenght(clientAddressLength);
					newClient.setServfd(_pollFds[i].fd);
					_clients_list.push_back(newClient);
					addClientToPoll(newClient);
				}
			}
			//Check Client Event
			else
			{
				Client& client = getClientByFd(_pollFds[i].fd);

				if (client.isTimeOut())
				{
					send(client.getSockfd(), timeOutPage().c_str(), timeOutPage().size(), 0);
					Log(ORANGE, "INFO", "Client timeout");
					client.resetClient();
					closeClientConnection(client);
				}
				else
				{
					if (_pollFds[i].revents & POLLIN)
					{
						// Log(CYAN, "INFO", "Read data...");
						readClientData(client);
					}

					/*
						Check client socket ready POLLOUT
					*/
					if (_pollFds[i].revents & POLLOUT && client.getEndRequest())
					{
						if (!client.getResponseBuild())
							buildResponse(client);
						client.sendResonse();
						if (client.getAllResponseSend())
							client.resetClient();
					}

					/*
						Check poll client probl
					*/
					if (_pollFds[i].revents & POLLERR || _pollFds[i].revents & POLLHUP)
						closeClientConnection(client);
				}
			}
			i++;
		}
	}
}

Server	ServerManager::getServerForRequest(Client& client)
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

void	ServerManager::buildResponse(Client& client)
{
	client.createRequest();
	Request request = client.getRequest();
	Server	server = getServerForRequest(client);
	Response response;

	Log(BRIGHT_BLUE, "INFO", "HTTP request received:");
	std::cout << PURPLE << request << RESET << std::endl;
	Exec(server, request, response);
	client.setResponse(response.getResponse());
	Log(ORANGE, "INFO", "Response:");
	std::cout << CYAN << response << RESET << std::endl;
	std::cout << "-------------------------------------------------\n" << std::endl;
}








