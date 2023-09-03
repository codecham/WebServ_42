/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcorenti <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 15:18:07 by dcorenti          #+#    #+#             */
/*   Updated: 2023/09/03 23:19:54 by dcorenti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP

#include "../../includes/webserv.hpp"
#include "../Server/Server.hpp"
#include "../Client/Client.hpp"
#include "../Request/Request.hpp"
#include "../Response/Response.hpp"
#include "../Methods/methods.hpp"
#include "../utils/Log.hpp"

#include <iostream>
#include <string>
#include <csignal>

class Server;
class Client;
class ServerManager
{
	public:
		ServerManager();
		ServerManager(std::list<Server> server_list);
		ServerManager(const ServerManager& copy);
		~ServerManager();
		ServerManager& operator=(const ServerManager& copy);


		void		runServers();

	private:
		std::list<Server> 					_server_list;
		std::map<int, std::vector<Server> > _serv_fds;
		std::map<int, Client>				_clients_list;
		std::vector<struct pollfd>			_pollFds;
		unsigned int						_nbServer;
		unsigned int						_nbClient;
		bool 								_run;

		void signalHandler(int signal) {
			if (signal == SIGINT)
				_run = false;
		}

		void	checkServers();
		void	createSockets();
		bool 	checkduplicate(std::list<Server>::iterator it);
		void	setServfds();
		void	addServToPoll();
		void	addClientToPoll(Client& client);
		void	closeClientConnection(Client& client);
		Client& getClientByFd(int fd);
		bool	readClientData(Client& client);
		void	buildResponse(Client& client);
		Server	getServerForRequest(Client& client);
		void	sendMaxClientPage(Client& client);
		void	closeServers();
		void	cleanClientList();
};

#endif
