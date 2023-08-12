/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcorenti <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 18:51:08 by dcorenti          #+#    #+#             */
/*   Updated: 2023/08/12 19:09:17 by dcorenti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response()
{
	_version = "HTTP/1.1";
	// _header.insert(std::make_pair("Connexion", "keep-alive"));
	create_mimes_types();
	
}

Response::Response(const Response& copy)
{
	_version = copy._version;
	_code = copy._code;
	_message = copy._message;
	_header = copy._header;
	_body = copy._body;
	_mimeTypes = copy._mimeTypes;
	_errors_pages = copy._errors_pages;
}

Response::~Response()
{

}

Response&	Response::operator=(const Response& copy)
{
	if (this != &copy)
	{
		_version = copy._version;
		_code = copy._code;
		_message = copy._message;
		_header = copy._header;
		_body = copy._body;
		_mimeTypes = copy._mimeTypes;
		_errors_pages = copy._errors_pages;
	}
	return(*this);
}

/*
	--------------------SETTERS----------------------	
*/

void	Response::setVersion(const std::string& str)
{
	_version = str;
}

void	Response::setCode(const std::string& str)
{
	_code = str;
	if (str == "100")
		_message = "Continue";
	else if (str == "101")
		_message = "Switching Protocols";
	else if (str == "200")
		_message = "OK";
	else if (str == "201")
		_message = "Created";
	else if (str == "204")
		_message = "No Content";
	else if (str == "301")
		_message = "Moved Permanently";
	else if (str == "302")
		_message = "Found";
	else if (str == "304")
		_message = "Not Modified";
	else if (str == "400")
		_message = "Bad Request";
	else if (str == "401")
		_message = "Unauthorized";
	else if (str == "403")
		_message = "Forbidden";
	else if (str == "404")
		_message = "Not Found";
	else if (str == "405")
		_message = "Method Not Allowed";
	else if (str == "500")
		_message = "Internal Server Error";
	else if (str == "502")
		_message = "Bad Gateway";
	else if (str == "503")
		_message = "Service Unavailable";
	else
		_message = "Unknow status code";
}

void	Response::setMessage(const std::string& str)
{
	_message = str;
}

void	Response::setHeader(const std::string& key, const std::string& value)
{
	_header.insert(std::make_pair(key, value));
}

void	Response::setBody(const std::string& str)
{
	_body = str;
	_header["Content-Length"] = to_string(str.size());
}

void	Response::setPage(const std::string& file)
{
	addFileToBody(file);
}

/*
	--------------------GETTERS----------------------	
*/

std::string	Response::getVersion() const
{
	return(_version);
}

std::string	Response::getCode() const
{
	return(_code);
}

std::string	Response::getMessage() const
{
	return(_message);
}

std::map<std::string, std::string>	Response::getHeader() const
{
	return(_header);
}

std::string	Response::getHeaderByKey(std::string key)
{
	std::map<std::string, std::string>::iterator it = _header.find(key);

	if (it != _header.end())
		return(_header[key]);
	else
		return("");
}

std::string	Response::getBody() const
{
	return(_body);
}

std::string Response::getResponse() const
{
	return(_response);
}

std::string Response::getResponseNoBody() const
{
	std::map<std::string, std::string>::const_iterator it = _header.begin();
	std::string response = _version + " " + _code + " " +_message + "\n";

	while(it != _header.end() && PRINT_RESP_HEADER)
	{
		response += it->first + ": " + it->second + "\n";
		it++;
	}
	return(response);
}

/*
	--------------MEMBER FUNCTIONS-------------------
*/

void	Response::buildResponse()
{
	std::map<std::string, std::string>::iterator it = _header.begin();

	_response += createline(_version + " " + _code + " " + _message);
	while (it != _header.end())
	{
		_response += createline(it->first + ": " + it->second);
		it++;
	}
	_response += ("\r\n");
	if (!_body.empty())
		_response += _body;
}

std::string Response::createline(std::string line)
{
	return(line + "\r\n");
}

void	Response::insert_mimes_types(std::string key, std::string value)
{
	_mimeTypes.insert(std::make_pair(key, value));
}

void	Response::create_mimes_types()
{
	insert_mimes_types(".aac", "audio/aac");
	insert_mimes_types(".abw", "application/x-abiword");
	insert_mimes_types(".arc", "application/octet-stream");
	insert_mimes_types(".avi", "video/x-msvideo");
	insert_mimes_types(".azw", "application/vnd.amazon.ebook");
	insert_mimes_types(".bin", "application/octet-stream");
	insert_mimes_types(".bmp", "image/bmp");
	insert_mimes_types(".bz", "application/x-bzip");
	insert_mimes_types(".bz2", "application/x-bzip2");
	insert_mimes_types(".css", "text/css");
	insert_mimes_types(".csv", "text/csv");
	insert_mimes_types(".doc", "application/msword");
	insert_mimes_types(".docx", "application/vnd.openxmlformats-officedocument.wordprocessingml.document");
	insert_mimes_types(".eot", "application/vnd.ms-fontobject");
	insert_mimes_types(".epub", "application/epub+zip");
	insert_mimes_types(".gif", "image/gif");
	insert_mimes_types(".html", "text/html");
	insert_mimes_types(".htm", "text/html");
	insert_mimes_types(".ico", "image/x-icon");
	insert_mimes_types(".ics", "text/calendar");
	insert_mimes_types(".jar", "application/java-archive");
	insert_mimes_types(".jpeg", "image/jpeg");
	insert_mimes_types(".jpg", "image/jpeg");
	insert_mimes_types(".js", "application/javascript");
	insert_mimes_types(".json", "application/json");
	insert_mimes_types(".mid", "audio/midi");
	insert_mimes_types(".midi", "audio/midi");
	insert_mimes_types(".mpeg", "video/mpeg");
	insert_mimes_types(".mpkg", "application/vnd.apple.installer+xml");
	insert_mimes_types(".odp", "application/vnd.oasis.opendocument.presentation");
	insert_mimes_types(".ods", "application/vnd.oasis.opendocument.spreadsheet");
	insert_mimes_types(".odt", "application/vnd.oasis.opendocument.text");
	insert_mimes_types(".oga", "audio/ogg");
	insert_mimes_types(".ogv", "video/ogg");
	insert_mimes_types(".ogx", "application/ogg");
	insert_mimes_types(".otf", "font/otf");
	insert_mimes_types(".png", "image/png");
	insert_mimes_types(".pdf", "application/pdf");
	insert_mimes_types(".ppt", "application/vnd.ms-powerpoint");
	insert_mimes_types(".pptx", "application/vnd.openxmlformats-officedocument.presentationml.presentation");
	insert_mimes_types(".rar", "application/x-rar-compressed");
	insert_mimes_types(".rtf", "application/rtf");
	insert_mimes_types(".sh", "application/x-sh");
	insert_mimes_types(".svg", "image/svg+xml");
	insert_mimes_types(".swf", "application/x-shockwave-flash");
	insert_mimes_types(".tar", "application/x-tar");
	insert_mimes_types(".tiff", "image/tiff");
	insert_mimes_types(".ts", "application/typescript");
	insert_mimes_types(".ttf", "font/ttf");
	insert_mimes_types(".vsd", "application/vnd.visio");
	insert_mimes_types(".wav", "audio/x-wav");
	insert_mimes_types(".weba", "audio/webm");
	insert_mimes_types(".webm", "video/webm");
	insert_mimes_types(".webp", "image/webp");
	insert_mimes_types(".woff", "font/woff");
	insert_mimes_types(".woff2", "font/woff2");
	insert_mimes_types(".xhtml", "application/xhtml+xml");
	insert_mimes_types(".xls", "application/vnd.ms-excel");
	insert_mimes_types(".xlsx", "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet");
	insert_mimes_types(".xml", "application/xml");
	insert_mimes_types(".xul", "application/vnd.mozilla.xul+xml");
	insert_mimes_types(".zip", "application/zip");
	insert_mimes_types(".7z", "application/x-7z-compressed");
}

void		Response::createResponse(const std::string& code, const std::string& file, Server& server)
{
	_errors_pages = server.getErrorPages();
	_server = server;
	cleanErrorsPage();
	setCode(code);
	setPage(file);
	buildResponse();
}

void		Response::createResponse(const std::string& code, Server& server)
{
	_errors_pages = server.getErrorPages();
	_server = server;
	cleanErrorsPage();
	if (code == "200" || code == "301")
		setCode(code);
	else
		setErrorPage(code);
	buildResponse();
}

void		Response::createResponseAutoIndex(const std::string& directory, Server& server)
{
	int status_code;
	
	_errors_pages = server.getErrorPages();
	_server = server;
	cleanErrorsPage();
	std::cout << "1" << std::endl;
	status_code = setAutoIndex(directory);
	if (status_code != 0)
		setErrorPage(to_string(status_code));
	buildResponse();
}

void		Response::addFileToBody(const std::string& fileName)
{
	std::ostringstream body;
	std::streampos fileSize;

	std::ifstream file(fileName.c_str(), std::ios::binary);

	if (!file)
	{
		setErrorPage("500");
		return;
	}
	file.seekg(0, std::ios::end);
	fileSize = file.tellg();
	file.seekg(0, std::ios::beg);
	std::vector<char> fileContent(fileSize);
	file.read(&fileContent[0], fileSize);
	file.close();
	_header.insert(std::make_pair("Content-Length", to_string(fileSize)));
	setMimeType(fileName);
	body.write(&fileContent[0], fileSize);
	_body = body.str();
}


void		Response::setMimeType(const std::string& fileName)
{
	std::map<std::string, std::string>::iterator it;
	std::string ext;
	std::size_t pos = fileName.find_last_of(".");

	if (pos != std::string::npos)
	{
		ext = fileName.substr(pos);
		it = _mimeTypes.find(ext);
		if (it != _mimeTypes.end())
			_header.insert(std::make_pair("Content-Type", it->second));
		else
			_header.insert(std::make_pair("Content-Type", "application/octet-stream"));
	}
	else
		_header.insert(std::make_pair("Content-Type", "application/octet-stream"));
}

void		Response::cleanErrorsPage()
{
	std::map<int, std::string>::iterator it = _errors_pages.begin();
	std::string root = _server.getRoot() + "/";
	
	while (it != _errors_pages.end())
	{
		if (!it->second.empty())
		{
			if (fileExist(root + it->second) && readRights(root + it->second))
				it->second = root + it->second;
			else
				it->second = "";
		}
		it++;
	}
}

void		Response::setErrorPage(const std::string& code)
{
	setCode(code);
	if (!_errors_pages[atoi(code.c_str())].empty())
		setPage(_errors_pages[atoi(code.c_str())]);
	else
	{
		_body = createErrorPage(_code, _message);
		_header.insert(std::make_pair("Content-Type", "text/html"));
		_header.insert(std::make_pair("Content-Length", to_string(_body.size())));
	}
}

int			Response::setAutoIndex(const std::string& path)
{
	DIR *dir;
	struct stat pathStat;
	std::multimap<int, std::string> contentMap;

	std::cout << "2" << std::endl;
	if (stat(path.c_str(), &pathStat) != 0) 
        return (404);
    if (!S_ISDIR(pathStat.st_mode)) 
        return (400);
	if (!readRights(path))
		return(403);
	dir = opendir(path.c_str());
	if (!dir)
		return(500);
	closedir(dir);
	contentMap = readDirectory(path);
	setCode("200");
	setHeader("Content-Type:", "text/html");
	_body = setHtmlAutoIndexPage(contentMap, path);
	return(0);
}


std::string Response::setHtmlAutoIndexPage(std::multimap<int, std::string>& contentMap, const std::string& path)
{
	std::string page;

	page += "<html>\n";
	page += "<head>\n";
	page += "<title> Index Of " + path + "</title>\n";
	page += "</head>\n";
	page += "<body>\n";
	page += "<h1> Index Of " + path + "</h1>\n";
	page += "<hr>\n";
	page += "<pre>\n";
	for (std::multimap<int, std::string>::iterator it = contentMap.begin(); it != contentMap.end(); ++it)
	{
		page += "<a href = \"";
        if (it->first == 0)
            page += it->second + "/";
        else if (it->first == 1)
            page += it->second;
		page += "\">";
        if (it->first == 0)
            page += it->second + "/";
        else if (it->first == 1)
            page += it->second;
		page += "</a>\n";
    }
	page += "</pre>\n";
	page += "<hr>\n";
	page += "</body>\n";
	page += "</html>\n";
	return(page);
}


/*------------------------------------------------*/

std::ostream& operator<<(std::ostream& os, const Response& response)
{
	if (PRINT_RESP_BODY)
		os << response.getResponse();
	else
		os << response.getResponseNoBody();
	return(os);
}