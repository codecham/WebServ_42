/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcorenti <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 23:35:35 by dcorenti          #+#    #+#             */
/*   Updated: 2023/09/04 04:44:55 by dcorenti         ###   ########.fr       */
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
#define BRIGHT_BLACK "\033[30;1m"
#define BRIGHT_RED   "\033[31;1m"
#define BRIGHT_GREEN "\033[32;1m"
#define BRIGHT_YELLOW "\033[33;1m"
#define BRIGHT_BLUE  "\033[34;1m"
#define BRIGHT_MAGENTA "\033[35;1m"
#define BRIGHT_CYAN  "\033[36;1m"
#define BRIGHT_WHITE "\033[37;1m"
#define ORANGE       "\033[38;5;208m"
#define PINK         "\033[38;5;205m"
#define PURPLE       "\033[38;5;135m"
#define LIME         "\033[38;5;118m"
#define TEAL         "\033[38;5;30m"
#define BROWN        "\033[38;5;130m"
#define SILVER       "\033[38;5;7m"
#define GOLD         "\033[38;5;220m"
#define GRAY         "\033[38;5;8m"


#define GET 0
#define POST 1
#define DELETE 2

#define MAX_CLIENTS 1
#define CONNECTION_TIMEOUT 30 // Time in seconds before client get kicked out if no data was sent.

#define	MAXREAD	50000
#define	MAXSEND	50000

#define PRINT_REQ_BODY 0
#define PRINT_RESP_BODY 0
#define PRINT_REQ_HEADER 0
#define PRINT_RESP_HEADER 0

#include <sstream>
#include <map>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <poll.h>
#include <signal.h>
#include <string>
#include <fstream>
#include <dirent.h>
#include <iostream>
#include <fcntl.h>
#include "../srcs/Request/Request.hpp"
#include "../srcs/utils/Log.hpp"

class Response;
class Request;
class Log;

/*--------------------------UTILS------------------------------*/
void 										strTrimedWhiteSpace(std::string& str);
bool 										emptyline(std::string& str);
std::vector<std::string> 					split_key_value(std::string& str);
std::map<int, std::vector<std::string> > 	extractFileInMap(std::ifstream& file);
std::vector<std::string>					splitInVector(std::string& str, char c);
bool 										fileExist(const std::string& path);
bool 										readRights(const std::string& path);
bool 										isDirectory(const std::string& path);
void 										sigPipeHandler(int sig);
std::string 								htmltestpage();
std::string 								timeOutPage();
void 										signalHandler(int sig);
std::vector<std::string>					splitInVectorByString(std::string& str, std::string cut);
std::vector<std::string> 					split_key_value_by_c(std::string& str, char c);
std::string 								createErrorPage(const std::string& code, const std::string& message);
std::multimap<int, std::string> 			readDirectory(const std::string& folderPath);
bool										pathIsDirectory(std::string& path);
int											setOptionSocket(int fd);
void 										splitString(const std::string& input, const std::string& delimiter, std::vector<std::string>& output);
int											createBinaryFileFromBody(Request& request, std::string directory);
std::string 								readingFile(std::ifstream& file);
std::string 								extractExtCgi(std::string path);
unsigned int 								fromHexToDec(const std::string& nb);
void										debugPrint(const std::string& str);
std::string 								decodeUrl(std::string str);


template <typename T>
std::string to_string(T value)
{
	std::ostringstream os;
	os << value;
	return (os.str());
}

#endif