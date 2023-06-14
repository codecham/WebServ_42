/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Log.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcorenti <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 23:35:27 by dcorenti          #+#    #+#             */
/*   Updated: 2023/06/03 20:16:11 by dcorenti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Log.hpp"

/*

	Just a little class for print Log. You can call where you want like this:

	1) Log("Message to print");
	2) Log("GREEN", "Message to print");
	3) Log("GREEN", "PARSING", message to print);

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

