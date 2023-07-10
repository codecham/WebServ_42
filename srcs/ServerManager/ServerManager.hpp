/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcorenti <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 15:18:07 by dcorenti          #+#    #+#             */
/*   Updated: 2023/07/02 23:49:27 by dcorenti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP

#include "../../includes/webserv.hpp"
#include "../Server/Server.hpp"
#include "../Client/Client.hpp"
#include "../Request/Request.hpp"
#include "../Response/Response.hpp"
#include "../Methods/Method.hpp"
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


		void	closeServers();
		void	printServfds();

	private:
		std::list<Server> 					_server_list;
		std::map<int, std::vector<Server> > _serv_fds;
		std::vector<int>					_servSocket;
		std::vector<Client>					_clients_list;

		void	checkServers();
		void	createSockets();
		bool 	checkduplicate(std::list<Server>::iterator it);
		void	runServers();
		void	setServfds();
		Server& getServByFd(int fd);
		void	checkpollServ(struct pollfd *poll_fds);
		void	addClientPoll(struct pollfd *poll_fds, Client& client);
		void	checkpollClient(struct pollfd *poll_fds);
		Client&	findClientByfd(int fd);
		void	addDataToClient(struct pollfd *poll_fds, int i, char *buffer, int bytesRead);
		void	checkClientRequest(struct pollfd *poll_fds);
		void	checkClientsTimeOut();
		void	clearClients(struct pollfd *pollfd);
		void	eraseClientsListByfd(int fd);
		void	checkRequest(int fd);
		Server  getServerForRequest(Client& client);
		void	execRequest(Client& client);
		void 	writeToClient(int client_fd, const std::string &str);
};

#endif