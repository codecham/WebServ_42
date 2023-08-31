/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcorenti <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 23:06:05 by dcorenti          #+#    #+#             */
/*   Updated: 2023/08/19 19:33:51 by dcorenti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "../../includes/webserv.hpp"
#include "../Request/Request.hpp"
#include "../utils/Log.hpp"

#include <iostream>
#include <string>
#include <cstring>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <cstdlib>

#define KEEP_ALIVE 1
#define CLOSE 2

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
		void setLastTime();
		void setResponse(std::string response);
		void setCloseConnexion();
		
		/*-----------GETTERS------------*/
		int 				getSockfd() const;
		int 				getServfd() const;
		struct sockaddr_in 	getClientAddress() const;
		socklen_t 			getClientAddressLenght() const;
		time_t				getLastTime() const;
		bool				getEndRequest() const;
		long long			getBodySize() const;
		std::string			getHeaderBody() const;
		Request				getRequest() const;
		std::string			getHeader() const;
		std::string			getBody() const;
		bool				getResponseBuild() const;
		bool				getAllResponseSend() const;


		/*--------MEMBER FUNCTION--------*/
		void				resetClient();
		void				separateHeaderBody();
		void 				setDataRecv(char *buf, int bytes);
		void				checkBodyOrChunk();
		void				checkConnexionType();
		void				checkEndRequest();
		void				printRequest();
		void				printResponse();
		void				unchunkBody();
		bool				isTimeOut();
		void				createRequest();
		void				reformResponse(int send);
		int					sendResonse();
		bool				isCloseConnexion();
		

	private:
		int 				_sockfd;
		int					_servfd;
		struct sockaddr_in 	_clientAddress;
		socklen_t			_clientAddressLenght;
		time_t				_lastTime;
		std::string			_tmp;
		std::string			_reqHeader;
		std::string			_reqBody;
		bool				_isChunk;
		bool				_hasBody;
		long long			_bodySize;
		bool				_initialRead;
		int					_connexion;
		bool				_endRequest;
		bool				_timeout;	
		Request				_request;
		std::string			_response;
		bool				_done;
		bool				_responseBuild;
};

#endif