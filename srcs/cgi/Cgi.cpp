#include "Cgi.hpp"

Cgi::Cgi()
{
	_statusCode = 0;
}

Cgi::Cgi(const Cgi& copy)
{
	_statusCode = copy._statusCode;
	_envMap = copy._envMap;
	_scriptName = copy._scriptName;
	_params = copy._params;
	_cgiDirectory = copy._cgiDirectory;
	_interpreter = copy._interpreter;
	_fullPath = copy._fullPath;
	_serverName = copy._serverName;
	_port = copy._port;
	_method = copy._method;
	_content_lenght = copy._content_lenght;
	_content_type = copy._content_type;
	_http_cookie = copy._http_cookie;
	_path = copy._path;
}

Cgi::~Cgi()
{

}

Cgi&	Cgi::operator=(const Cgi& copy)
{
	if (this != &copy)
	{
		_statusCode = copy._statusCode;
		_envMap = copy._envMap;
		_scriptName = copy._scriptName;
		_params = copy._params;
		_cgiDirectory = copy._cgiDirectory;
		_interpreter = copy._interpreter;
		_fullPath = copy._fullPath;
		_serverName = copy._serverName;
		_port = copy._port;
		_method = copy._method;
		_content_lenght = copy._content_lenght;
		_content_type = copy._content_type;
		_http_cookie = copy._http_cookie;
		_path = copy._path;
	}
	return(*this);
}

void	Cgi::initCgi(Server& server, Request& request, Location& location)
{
	extractParam(request);
	_params = decode(_params);
	setPath(request.getPath());
	extractScriptName(request.getPath());
	setDirectory(location);
	setInterpret(location);
	setFullPath();
	setServerData(server);
	_method = request.getMethod();
	if (request.getBodySize() <= 0)
		_content_lenght = "";
	else
		_content_lenght = to_string(request.getBodySize());
	_content_type = request.getHeaderByKey("Content-Type");
	_http_cookie = request.getHeaderByKey("Cookie");
	setEnvMap();
}

void	Cgi::extractScriptName(std::string path)
{
	size_t nFind;

	nFind = path.find("?");
	if (nFind != std::string::npos)
		path = path.substr(0, nFind);
	nFind = path.find_last_of("/");
	if (nFind == std::string::npos)
		return(setStatusCode(400, "Bad request"));
	_scriptName = path.substr(nFind + 1);
}

void	Cgi::extractParam(Request& request)
{
	size_t nFind;
	std::string path = request.getPath();

	if (request.getMethod() == "GET")
	{

		nFind = path.find("?");

		if (nFind == std::string::npos)
			return(setStatusCode(400, "Bad request"));
		if (nFind >= path.length())
			return;

		_params = path.substr(nFind + 1);

	}
	else
		_params = request.getBody();
}

void	Cgi::setDirectory(Location& location)
{
	char 		buf[1024];
	std::string tmp;

	if (getcwd(buf, sizeof(buf)) == NULL)
		return(setStatusCode(500, "getCwd failed"));
	_cgiDirectory = buf;
	tmp = location.getCgiPath();
	if (tmp.empty())
		_cgiDirectory += "/cgi-bin/";
	else
	{
		if (tmp[0] != '/')
			_cgiDirectory += "/";
		_cgiDirectory += tmp;
		if (_cgiDirectory[_cgiDirectory.size() - 1] != '/')
			_cgiDirectory += "/";
	}
	
}

void	Cgi::setInterpret(Location& location)
{
	std::string ext = extractExtCgi(_scriptName);

	_interpreter = location.getCgiInterpreter(ext);
	if (_interpreter.empty())
		return(setStatusCode(500, "Interpreter for " + _scriptName + " not found"));
	if (!fileExist(_interpreter))
		return(setStatusCode(500, "Can't find " + _interpreter));
}

void	Cgi::setStatusCode(int code, const std::string& message)
{
	if (_statusCode != 0)
	{
		_statusCode = code;
		Log(RED, "CGI", message);
	}
}

void	Cgi::setFullPath()
{
	_fullPath += _cgiDirectory;
	_fullPath += _scriptName;
}


void	Cgi::printEnv() const
{
	std::map<std::string, std::string>::const_iterator it = _envMap.begin();

	if (_envMap.empty())
		return ;
	std::cout << "envMap:" << std::endl;
	while (it != _envMap.end())
	{
		std::cout << "[" << it->first << "] - [" << it->second << "]" << std::endl;
		it++;
	}
}

void	Cgi::setServerData(Server& server)
{
	if (server.getName().empty())
		_serverName = "localhost";
	else
		_serverName = server.getName();
	_port = to_string(server.getPort());
}

void	Cgi::setEnvMap()
{
	_envMap["AUTH_TYPE"] = "Basic";
	_envMap["CONTENT_LENGTH"] = _content_lenght;
	_envMap["CONTENT_TYPE"] = _content_type;
	_envMap["GATEWAY_INTERFACE"] = "CGI/1.1";
	_envMap["PATH_INFO"] = "";
	_envMap["PATH_TRANSLATED"] = _fullPath;
	_envMap["QUERY_STRING"] = _params;
	_envMap["HTTP_COOKIE"] = _http_cookie;
	_envMap["REDIRECT_STATUS"] = "200";
	_envMap["REMOTE_ADDR"] = _serverName + ":" + _port;
	_envMap["REQUEST_METHOD"] = _method;
	_envMap["REQUEST_URI"] = _path + _params;
	_envMap["SCRIPT_FILENAME"] = _scriptName;
	_envMap["SCRIPT_NAME"] = _path ;
	_envMap["SERVER_NAME"] = _serverName;
	_envMap["SERVER_PORT"] = _port;
	_envMap["SERVER_PROTOCOL"] = "HTTP/1.1";
	_envMap["SERVER_SOFTWARE"] = "WebServ/1.0" ;
}

char		**Cgi::getEnvAsCstrArray() const 
{
	char	**env = new char*[this->_envMap.size() + 1];
	int	j = 0;
	for (std::map<std::string, std::string>::const_iterator i = this->_envMap.begin(); i != this->_envMap.end(); i++) 
	{
		std::string	element = i->first + "=" + i->second;
		env[j] = new char[element.size() + 1];
		env[j] = strcpy(env[j], (const char*)element.c_str());
		j++;
	}
	env[j] = NULL;
	return env;
}

std::string	Cgi::executeCgi()
{
	pid_t		pid;
	int			saveStdin;
	int			saveStdout;
	char		**env;
	std::string newBody;
	
	char* args[] = {const_cast<char*>(_interpreter.c_str()), const_cast<char*>(_fullPath.c_str()), NULL};

	try 
	{
		env = getEnvAsCstrArray();
	}
	catch (std::bad_alloc &e)
	{
		std::cerr << RED << e.what() << RESET << std::endl;
		return ("Status: 500\r\n\r\n");
	}


	saveStdin = dup(STDIN_FILENO);
	saveStdout = dup(STDOUT_FILENO);


	FILE	*fIn = tmpfile();
	FILE	*fOut = tmpfile();
	long	fdIn = fileno(fIn);
	long	fdOut = fileno(fOut);
	int		ret = 1;


	write(fdIn, _params.c_str(), _params.size());
	lseek(fdIn, 0, SEEK_SET);

	pid = fork();


	if (pid == -1)
	{
		std::cerr << RED << "Fork Crashed" << RESET << std::endl;
		return ("Status: 500\r\n\r\n");
	}
	else if (!pid)
	{
		dup2(fdIn, STDIN_FILENO);
		dup2(fdOut, STDOUT_FILENO);
		execve(_interpreter.c_str(), args, env);
		std::cerr << RED << "Execve Crashed" << RESET << std::endl;
		write(STDOUT_FILENO, "Status: 500\r\n\r\n", 15);
	}
	else
	{
		char	buffer[CGI_BUFSIZE] = {0};

		waitpid(-1, NULL, 0);
		lseek(fdOut, 0, SEEK_SET);

		ret = 1;
		while (ret > 0)
		{
			memset(buffer, 0, CGI_BUFSIZE);
			ret = read(fdOut, buffer, CGI_BUFSIZE - 1);
			newBody += buffer;
		}
	}
	dup2(saveStdin, STDIN_FILENO);
	dup2(saveStdout, STDOUT_FILENO);
	fclose(fIn);
	fclose(fOut);
	close(fdIn);
	close(fdOut);
	close(saveStdin);
	close(saveStdout);

	for (size_t i = 0; env[i]; i++)
		delete[] env[i];
	delete[] env;

	if (!pid)
		exit(0);
	return (newBody);
}

void	Cgi::printCgi() const
{
	std::cout << "scriptName = [" << _scriptName << "]" << std::endl;
	std::cout << "params = [" << _params << "]" << std::endl;
	std::cout << "cgiDirectory = [" << _cgiDirectory << "]" << std::endl;
	std::cout << "interpreter = [" << _interpreter << "]" << std::endl;
	std::cout << "fullPath = [" << _fullPath << "]" << std::endl;
	std::cout << "serverName = [" << _serverName << "]" << std::endl;
	std::cout << "port = [" << _port << "]" << std::endl;
	std::cout << "method = [" << _method << "]" << std::endl;
	std::cout << "content_lenght = [" << _content_lenght << "]" << std::endl;
	std::cout << "content_type = [" << _content_type << "]" << std::endl;
	printEnv();
}

std::string Cgi::decode(std::string str)
{
	size_t token = str.find("%");
	while (token != std::string::npos)
	{
		if (str.length() < token + 2)
			break ;
		char decimal = fromHexToDec(str.substr(token + 1, 2));
		str.replace(token, 3, to_string(decimal));
		token = str.find("%");
	}
	return (str);
}

void		Cgi::setPath(std::string path)
{
	size_t nFind;

	nFind = path.find("?");
	if (nFind != std::string::npos)
		_path = path.substr(0, nFind);
	else
		_path = path;
}

int	Cgi::getStatusCode() const
{
	return(_statusCode);
}