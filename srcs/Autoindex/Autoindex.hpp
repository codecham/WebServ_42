#ifndef AUTOINDEX_HPP
#define AUTOINDEX_HPP

#include <iostream>
#include <string>
#include "../../includes/webserv.hpp"

class Autoindex
{
	public:
		Autoindex();
		Autoindex(std::string path);
		Autoindex(const Autoindex& copy);
		~Autoindex();
		Autoindex& operator=(const Autoindex& copy);

		/*--------SETTERS--------*/
		void setPath(std::string str);
		void sethtmlPage(std::string str);

		/*--------GETTER---------*/
		std::string getPath() const;
		std::string getHtmlPage() const;
		int getErrorCode() const;

	private:
		int							_error_code;
		std::vector<std::string> 	_files;
		std::string 				_path;
		std::string 				_htmlPage;
};

#endif