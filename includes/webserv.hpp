/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcorenti <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 23:35:35 by dcorenti          #+#    #+#             */
/*   Updated: 2023/06/03 22:33:32 by dcorenti         ###   ########.fr       */
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

#include "../utils/Log.hpp"
#include "../ParserConfig/ParserConfig.hpp"
#include "../Server/Server.hpp"
#include "../Server/Location.hpp"

#include <map>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

/*--------------------------UTILS------------------------------*/
void strTrimedWhiteSpace(std::string& str);
bool emptyline(std::string& str);
std::vector<std::string> split_key_value(std::string& str);
std::map<int, std::vector<std::string> > extractFileInMap(std::ifstream& file);
std::vector<std::string>	splitInVector(std::string& str, char c);
bool pathToFileExist(const std::string& path);
bool isDirectory(const std::string& path);

#endif