/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcorenti <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 12:48:52 by dcorenti          #+#    #+#             */
/*   Updated: 2023/05/25 20:53:59 by dcorenti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/webserv.hpp"

bool emptyline(std::string& str)
{
	std::size_t token;

	if (str.empty())
		return(true);
	token = str.find_first_not_of(" \t");
	if (token == std::string::npos)
		return(true);
	return(false);
}

std::vector<std::string>	splitInVector(std::string& str, char c)
{
	std::vector<std::string> split;
	int i;

	i = 0;
	i = str.find(c);
	while (i != -1)
	{
		split.push_back(str.substr(0, i));
		str.erase(0, i + 1);
		i = str.find(c);
	}
	split.push_back(str);
	return(split);
}

void strTrimedWhiteSpace(std::string& str)
{
	std::size_t first_char_pos = str.find_first_not_of(" \t");
	std::size_t last_char_pos = str.find_last_not_of(" \t");

	if (first_char_pos == std::string::npos && last_char_pos == std::string::npos)
		str = "";
	else if(first_char_pos != std::string::npos && last_char_pos != std::string::npos)
		str = str.substr(first_char_pos, last_char_pos - first_char_pos + 1);
	else if(first_char_pos != std::string::npos)
		str = str.substr(first_char_pos);
	else if (last_char_pos != std::string::npos)
		str = str.substr(0, last_char_pos - str.size());
}

std::vector<std::string> split_key_value(std::string& str)
{
	std::vector<std::string> vec;
	std::size_t first_space_pos;

	strTrimedWhiteSpace(str);
	first_space_pos = str.find_first_of(" ");
	if (first_space_pos == std::string::npos)
	{
		vec.push_back(str);
		vec.push_back("");
	}
	else
	{
		vec.push_back(str.substr(0, first_space_pos));
		vec.push_back(str.substr(first_space_pos + 1, str.size() - first_space_pos - 1));
		strTrimedWhiteSpace(vec[1]);
	}
	return(vec);
}

std::map<int, std::vector<std::string> > extractFileInMap(std::ifstream& file)
{
	std::map<int, std::vector<std::string> > map;
	std::vector<std::string> split;
	std::string line;
	int i = 1;

	while (std::getline(file, line))
	{
		if (!emptyline(line))
			map.insert(std::make_pair(i, split_key_value(line)));
		i++;
	}
	return(map);
}

bool pathToFileExist(const std::string& path) 
{
	std::ifstream file(path.c_str());
	return (file.is_open());
}

bool isDirectory(const std::string& path) 
{
	struct stat filestat;
	if (stat(path.c_str(), &filestat) != 0)
		return false;
	return S_ISDIR(filestat.st_mode);
}