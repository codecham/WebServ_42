/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Log.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcorenti <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 23:35:27 by dcorenti          #+#    #+#             */
/*   Updated: 2023/06/18 00:33:04 by dcorenti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Log.hpp"

/*

	Just a little class for print Log. You can call where you want like this:

	1) Log("Message to print");
	2) Log("GREEN", "Message to print");
	3) Log("GREEN", "PARSING", "Message to print");

*/

Log::Log(const std::string msg)
{
	struct timeval current_time;
	char time_buffer[100];
	bzero(time_buffer, 100);
	gettimeofday(&current_time, NULL);
	strftime(time_buffer,100,"%F %R:%S", localtime(&current_time.tv_sec));
	std::cout << "[" << time_buffer << "] : " << msg << std::endl;
}

Log::Log(const std::string color, const std::string msg)
{
	struct timeval current_time;
	char time_buffer[100];
	bzero(time_buffer, 100);
	gettimeofday(&current_time, NULL);
	strftime(time_buffer,100,"%F %R:%S", localtime(&current_time.tv_sec));
	std::cout << color << "[" << time_buffer << "] : " << msg << RESET << std::endl;
}

Log::Log(const std::string color, const std::string level, const std::string msg)
{
	struct timeval current_time;
	char time_buffer[100];
	bzero(time_buffer, 100);
	gettimeofday(&current_time, NULL);
	strftime(time_buffer,100,"%F %R:%S", localtime(&current_time.tv_sec));
	std::cout << color << "[" << time_buffer << "] [" << level << "] : " << msg << RESET << std::endl;
}

Log::Log(const std::string color, const Server serv, const std::string msg)
{
	struct timeval current_time;
	struct in_addr addr;

	addr.s_addr = serv.getHost();
	char time_buffer[100];
	bzero(time_buffer, 100);
	gettimeofday(&current_time, NULL);
	strftime(time_buffer,100,"%F %R:%S", localtime(&current_time.tv_sec));
	std::cout << color;
	std::cout << "\t[" << time_buffer << "]\t";
	std::cout << "[SERVER " << inet_ntoa(addr) << ":" << serv.getPort() << "] ";
	std::cout << msg << RESET << std::endl;	
}

Log::Log(const std::string color, const Server serv, struct sockaddr_in clientAddress, socklen_t clientAddressLength, const std::string msg)
{
	struct timeval current_time;
	struct in_addr addr;

	addr.s_addr = serv.getHost();
	char time_buffer[100];
	bzero(time_buffer, 100);
	gettimeofday(&current_time, NULL);
	strftime(time_buffer,100,"%F %R:%S", localtime(&current_time.tv_sec));
	std::cout << color;
	std::cout << "[" << time_buffer << "]";
	std::cout << "\t[SERVER " << inet_ntoa(addr) << ":" << serv.getPort() << "]\t";
	std::cout << msg;
	std::cout << " IP address: " << inet_ntoa(clientAddress.sin_addr);
	std::cout << " Port: " << ntohs(clientAddress.sin_port) << RESET << std::endl;
	(void)clientAddressLength;
	
}

Log::Log(const Log& copy)
{
	(void)copy;
}

Log::~Log(void)
{

}

Log&	Log::operator=(const Log& copy)
{
	(void)copy;
	return(*this);
}

