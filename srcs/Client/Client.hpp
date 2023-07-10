/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcorenti <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 23:06:05 by dcorenti          #+#    #+#             */
/*   Updated: 2023/07/05 00:44:36 by dcorenti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "../../includes/webserv.hpp"
#include "../Request/Request.hpp"

#include <iostream>
#include <string>
#include <cstring>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

class Request;

class Client
{
	public:
		Client();
		Client(const Client& copy);
		~Client();
		Client& operator=(const Client& copy);


		/*-----------SETTERS------------*/
		void setSockfd(int fd);
		void setServfd(int fd);
		void setClientAddress(struct sockaddr_in clientAddress);
		void setClientAdressLenght(socklen_t clientAddressLenght);
		void setDataRecv(std::string data);
		void setLastTime();
		void setInactive();
		void setRequest(Request& request);
		void setRequestDone(bool value);


		/*-----------GETTERS------------*/
		int 				getSockfd() const;
		int 				getServfd() const;
		struct sockaddr_in 	getClientAddress() const;
		socklen_t 			getClientAddressLenght() const;
		std::string			getDataRecv() const;
		time_t				getLastTime() const;
		bool				getActive() const;
		unsigned long		getBodySize() const;
		Request				getRequest() const;
		bool 				getRequestDone() const;


		/*--------MEMBER FUNCTION--------*/
		bool				isHTTPrequest();
		bool				allDataReceive();
		bool				requestFilled();
		void				closefd();
		void				setBodySize(std::string content_lenght_line);
		bool				checkBodySize(const char *end_header);
		void 				createRequest();

	private:
		int 				_sockfd;
		int					_servfd;
		struct sockaddr_in 	_clientAddress;
		socklen_t			_clientAddressLenght;
		std::string			_dataRecv;
		time_t				_lastTime;
		bool				_active;
		unsigned long		_bodySize;
		Request				_request;
		bool				_requestDone;
};

std::ostream& operator<<(std::ostream& os, const Client& client);

#endif