/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcorenti <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 23:35:35 by dcorenti          #+#    #+#             */
/*   Updated: 2023/06/30 19:44:57 by dcorenti         ###   ########.fr       */
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

#define MAX_CLIENTS 1024
#define CONNECTION_TIMEOUT 3 // Time in seconds before client get kicked out if no data was sent.
#define DEFAULT_ERROR_PAGE_DIRECTORY "files/errors_pages/"

#include "../srcs/utils/Log.hpp"
#include "../srcs/ParserConfig/ParserConfig.hpp"
#include "../srcs/Server/Server.hpp"
#include "../srcs/Server/Location.hpp"
#include "../srcs/Request/Request.hpp"
#include "../srcs/ServerManager/ServerManager.hpp"
#include "../srcs/Client/Client.hpp"
#include "../srcs/Response/Response.hpp"

#include <sstream>
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

class Response;
/*--------------------------UTILS------------------------------*/
void strTrimedWhiteSpace(std::string& str);
bool emptyline(std::string& str);
std::vector<std::string> split_key_value(std::string& str);
std::map<int, std::vector<std::string> > extractFileInMap(std::ifstream& file);
std::vector<std::string>	splitInVector(std::string& str, char c);
bool fileExist(const std::string& path);
bool readRights(const std::string& path);
bool isDirectory(const std::string& path);
void sigPipeHandler(int sig);
std::string htmltestpage();
std::string timeOutPage();
void signalHandler(int sig);
std::vector<std::string>	splitInVectorByString(std::string& str, std::string cut);
std::vector<std::string> 	split_key_value_by_c(std::string& str, char c);
std::string 				extractFileToString(std::string file_name);
std::string 				extractFileBinary(std::string file_name);
void						testExec(Server& server, Request& request, Response& response);
std::string 				createHTTPResponseWithImage(const std::string& imageFileName);
void writeToLogFile(const std::string& text);

/*--------------------------UTILS 2------------------------------*/

template <typename T>
std::string to_string(T value)
{
	std::ostringstream os;
	os << value;
	return (os.str());
}

#endif