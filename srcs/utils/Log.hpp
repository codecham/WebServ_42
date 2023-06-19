/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Log.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcorenti <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 23:27:44 by dcorenti          #+#    #+#             */
/*   Updated: 2023/06/16 19:21:33 by dcorenti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOG_HPP
#define LOG_HPP

#include <iostream>
#include <string>
#include <ctime>
#include <sys/time.h>
#include <sys/socket.h>
#include "../../includes/webserv.hpp"

class Server;
class Log
{
	public:
		Log(const std::string msg);
		Log(const std::string level, const std::string msg);
		Log(const std::string color, const std::string level, const std::string msg);
		Log(const std::string color, const Server serv, const std::string msg);
		Log(const std::string color, const Server serv, struct sockaddr_in clientAddress, socklen_t clientAddressLength, const std::string msg);
		~Log();

	private:
		Log();
		Log(const Log& copy);
		Log& operator=(const Log& copy);

};

#endif