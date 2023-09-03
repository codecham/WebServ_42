/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcorenti <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 12:48:52 by dcorenti          #+#    #+#             */
/*   Updated: 2023/09/03 23:04:42 by dcorenti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Request/Request.hpp"
#include "../../includes/webserv.hpp"

/*
	Return true if the string is empty or just contains space or tabulation
*/

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


/*
	Works like ft_split in libft.
	Cut a string on each character c and put the each part in a vector
*/

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


/*
	Pretty the same than other version but with a string in place of only one char
	Cut a string on each character c and put the each part in a vector
*/

std::vector<std::string>	splitInVectorByString(std::string& str, std::string cut)
{
	std::vector<std::string> split;
	int i;

	i = 0;
	i = str.find(cut);
	while (i != -1)
	{
		split.push_back(str.substr(0, i));
		str.erase(0, i + cut.size());
		i = str.find(cut);
	}
	split.push_back(str);
	return(split);
}


void 					splitString(const std::string& input, const std::string& delimiter, std::vector<std::string>& output) 
{
    size_t start = 0;
    size_t end = input.find(delimiter);

    while (end != std::string::npos) {
        output.push_back(input.substr(start, end - start));
        start = end + delimiter.size();
        end = input.find(delimiter, start);
    }
    output.push_back(input.substr(start));
}

/*
	Take a string and remove all spaces and tabulations at begin and at the end 
*/

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

/*
	Split a combination of KEY VALUE speratate by space
*/

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


/*
	Split a combination of KEY VALUE speratate by a char
*/

std::vector<std::string> split_key_value_by_c(std::string& str, char c)
{
	std::vector<std::string> vec;
	std::size_t first_space_pos;

	strTrimedWhiteSpace(str);
	first_space_pos = str.find_first_of(c);
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


/*
	Just a function used for the parsing
*/

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


/*
	take a file path and return true if the reading right is ok
*/

bool readRights(const std::string& path) 
{
	struct stat filestat;
	if (stat(path.c_str(), &filestat) != 0)
		return false;
	if (filestat.st_mode & S_IRUSR)
		return(true);
	return(false);
}

/*
	take a file path and return true if it's exist
*/

bool fileExist(const std::string& path)
{
	if (access(path.c_str(), F_OK) == 0)
		return(true);
	return (false);
}

/*
	take a directory path and return true if it's exist
*/

bool isDirectory(const std::string& path) 
{
	struct stat filestat;
	if (stat(path.c_str(), &filestat) != 0)
		return false;
	return S_ISDIR(filestat.st_mode);
}


/*
	SIGPIPE is a signal generated when something tries to write to a socket whose read side is closed.

	If this signal is not managed, the program closes.

	So the goal of this function is just to ignore this signal without closing the program
*/

void sigPipeHandler(int sig)
{
	(void)sig;
}

/*
	This is just a function who's create a test HTML page
	add this for get style.css \n<link rel=\"stylesheet\" type=\"text/css\" href=\"styles.css\">
*/

std::string htmltestpage()
{
    std::string response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: text/html\r\n";
	response += "Content-Length: 91\r\n";
    response += "\r\n";
    response += "<html><head>\n</head>\n<body><h1>Hello from webserv! I'm just a test response</p></body></html>";
    return response;
	return(response);
}

std::string timeOutPage() 
{
    std::string response;
    response += "HTTP/1.1 408 Request Timeout\r\n"
                "Content-Type: text/html\r\n"
                "Connection: close\r\n"
                "\r\n"
                "<!DOCTYPE html>\n"
                "<html>\n"
                "<head>\n"
                "<title>Error Page</title>\n"
                "</head>\n"
                "<body>\n"
                "<h1>408 Request Time-out</h1>\n"
                "</body>\n"
                "</html>";
    return response;
}


std::string createErrorPage(const std::string& code, const std::string& message)
{
	std::string response;
    response += "<!DOCTYPE html>\n"
                "<html>\n"
                "<head>\n"
                "<title>Error Page</title>\n"
                "</head>\n"
                "<body>\n";
    response += "<h1>";
	response += code + " ";
	response += message;
	response += "</h1>\n";
    response += "</body>\n"
                "</html>";
    return response;
}

std::multimap<int, std::string> readDirectory(const std::string& folderPath) 
{
    std::multimap<int, std::string> contentMap;
    
    DIR* directory = opendir(folderPath.c_str());
    if (directory) {
        dirent* entry;
        while ((entry = readdir(directory)) != NULL) {
            std::string name = entry->d_name;
            
            if (name != ".") {
                std::string fullPath = folderPath + "/" + name;
                
                if (entry->d_type == DT_DIR) {
                    contentMap.insert(std::make_pair(0, name));
                } else if (entry->d_type == DT_REG) {
                    contentMap.insert(std::make_pair(1, name));
                }
            }
        }
        
        closedir(directory);
    } else {
        std::cout << "Erreur lors de l'ouverture du dossier." << std::endl;
    }
    
    return contentMap;
}

bool	pathIsDirectory(std::string& path)
{
	if (path.empty())
		return(false);
	if (path[path.size() - 1] == '/')
		return(true);
	return(false);
}

int	setOptionSocket(int fd)
{
	fcntl(fd, F_SETFL,	O_NONBLOCK);
	return 0;
}

void	extractFileName(std::string& information, std::string& fileName)
{
	size_t start;
	size_t end;

	start = information.find("filename=");
	start += 10;
	end = information.find("\"", start);
	fileName = information.substr(start, end - start);
}

void	separateInformationsBinary(std::string body, std::string& fileName, std::string& binary, std::string separator)
{
	size_t nFind;
	size_t httpSepPos;
	std::string information;
	
	nFind = body.find(separator);
	httpSepPos = body.find("\r\n\r\n");
	information = body.substr((nFind + separator.size()), httpSepPos);
	extractFileName(information, fileName);
	httpSepPos += 4;
	binary = body.substr(httpSepPos);
}

void	getSeparator(Request& request, std::string& separator)
{
	size_t nFind;
	
	separator = request.getHeaderByKey("Content-Type");
	nFind = separator.find("boundary=");
	separator = separator.substr(nFind + 9);
	
}

int	createBinaryFileFromBody(Request& request, std::string directory)
{
	std::string separator;
	std::string fileName;
	std::string binary;
	std::ofstream file;
	
	getSeparator(request, separator);
	separateInformationsBinary(request.getBody(), fileName, binary, separator);
	fileName = directory + fileName;
	file.open(fileName, std::ios::binary);
	if (!file)
	{
		std::cout << RED << "Can't create " << fileName << RESET << std::endl;
		return(500);
	}
	file.write(binary.c_str(), binary.size());
	file.close();
	Log(GREEN, "INFO", fileName + " has been successfully uploaded");
	return(200);
}

std::string readingFile(std::ifstream& file)
{
	std::string line;
	std::string data;

	while(std::getline(file, line))
		data += line;
	return(data);
}

std::string extractExtCgi(std::string path)
{
	size_t nFind;

	nFind = path.find_last_of("?");
	if (nFind != std::string::npos)
		path = path.substr(0, nFind);
	nFind = path.find_last_of(".");
	if (nFind == std::string::npos)
		return("");
	return(path.substr(nFind));
}


unsigned int fromHexToDec(const std::string& nb)
{
	unsigned int x;
	std::stringstream ss;
	ss << nb;
	ss >> std::hex >> x;
	return (x);
}

void	debugPrint(const std::string& str)
{
	if (DEBUGMESS)
		std::cout << str << std::endl;
}


void	printFdMax()
{
	struct rlimit rlim;

	getrlimit(RLIMIT_NOFILE, &rlim);
	std::cout << "max Open fd = " << rlim.rlim_cur << std::endl;
}

void	setOpenMaxFd()
{
	struct rlimit rlim;
    
    rlim.rlim_cur = 4096;
    rlim.rlim_max = 4096;

    if (setrlimit(RLIMIT_NOFILE, &rlim) == 0)
        std::cout << "Max open file descriptors updated" << std::endl;
	else 
        std::cerr << "Error updating max open file descriptors" << std::endl;
}