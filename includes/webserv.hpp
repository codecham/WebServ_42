/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcorenti <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 23:35:35 by dcorenti          #+#    #+#             */
/*   Updated: 2023/06/19 02:26:01 by dcorenti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#define RESET   "\033[0m"
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN    "\033[1;36m"
#define GREY    "\033[38;5;8m"

#define GET 0
#define POST 1
#define DELETE 2

#define MAX_CLIENTS 1024
#define CONNECTION_TIMEOUT 3 // Time in seconds before client get kicked out if no data was sent.

#include "../srcs/utils/Log.hpp"
#include "../srcs/ParserConfig/ParserConfig.hpp"
#include "../srcs/Server/Server.hpp"
#include "../srcs/Server/Location.hpp"
#include "../srcs/Request/Request.hpp"
#include "../srcs/ServerManager/ServerManager.hpp"
#include "../srcs/Client/Client.hpp"


#include <map>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <poll.h>
#include <signal.h>



/*--------------------------UTILS------------------------------*/
void strTrimedWhiteSpace(std::string& str);
bool emptyline(std::string& str);
std::vector<std::string> split_key_value(std::string& str);
std::map<int, std::vector<std::string> > extractFileInMap(std::ifstream& file);
std::vector<std::string>	splitInVector(std::string& str, char c);
bool pathToFileExist(const std::string& path);
bool isDirectory(const std::string& path);
void sigPipeHandler(int sig);
std::string htmltestpage();
std::string timeOutPage();
void signalHandler(int sig);
std::vector<std::string>	splitInVectorByString(std::string& str, std::string cut);
std::vector<std::string> 	split_key_value_by_c(std::string& str, char c);

/*--------------------------UTILS 2------------------------------*/

/*
	You can add here your functions that you code in the file utils/utils2.cpp :)

*/

#endif