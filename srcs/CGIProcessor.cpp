#include "CGIProcessor.hpp"


CGIProcessor::CGIProcessor(Request *req, Response *res) : _parsedRequest(req), _response(res), _env(9)
{
    this->_path_to_script = "./var/www/cgi-bin/hello_get.py";
    _env.createEnv(*_parsedRequest, _path_to_script);
}


void CGIProcessor::handleCGIRequest(int client_fd)
{
			printf("******************2**********************\n\n");
			std::cout << "Print req.getBody()" << std::endl;
			std::cout << _parsedRequest->getBody() << std::endl;
			std::cout << "Print req.getBody().length()" << std::endl;
			std::cout << _parsedRequest->getBody().length() << std::endl;
			printf("****************************************\n\n");

			char * _argv[] = {const_cast<char*>(_path_to_script.c_str()), NULL}; 

			if (_parsedRequest->getMethod() == "GET") {
				handleGETCGI(client_fd, _argv);
			}
			else if (_parsedRequest->getMethod() == "POST") {
				handlePOSTCGI(client_fd, _argv);	
			}
}

std::string sizeToString_CGI(size_t value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

void CGIProcessor::handleGETCGI(int client_fd, char * _argv[])
{

			// pipe for communication between parent and child
			int pipefd[2];
			if (pipe(pipefd) == -1) {
				throw std::runtime_error("pipe");
			}
			pid_t pid = fork();
			if (pid < 0) {
				throw std::runtime_error("fork");
			}
			else if (pid == 0)  // child
			{
				printf("this is a GET child process\n");
				//*** GET request ***
				std::cout << "PATH: " << _path_to_script << std::endl;
				close(pipefd[0]);
				dup2(pipefd[1], STDOUT_FILENO);
				close(pipefd[1]);
				if (execve(_path_to_script.c_str(), _argv, _env.getEnv()) == -1)
					printf("EXECVE FAILURE\n");
			}
			else // parent
			{
			
				close(pipefd[1]);
				char buf[10000];
				int nbytes = read(pipefd[0], buf, sizeof(buf)); // don't forget to do it in a loop later
				if (nbytes == -1) {
					throw std::runtime_error("read");
				}
				buf[nbytes] = '\0';
				printf("buf: %s\n", buf);
				_response->setStatus(200, "OK");
				_response->addHeader("Content-Type", "text/html");
				_response->addHeader("Content-Length", sizeToString_CGI(nbytes));
				_response->setBody(buf);
				std::string str = _response->serialize();
				if (send(client_fd, str.c_str(), str.length(), 0) == -1)
					throw std::runtime_error(strerror(errno));
				close(pipefd[0]);
				int status;
				waitpid(pid, &status, 0);
			}
}

void CGIProcessor::handlePOSTCGI(int client_fd, char * _argv[])
{

		// pipe for communication between parent and child
	int pipefd[2];
	if (pipe(pipefd) == -1) {
		throw std::runtime_error("pipe");
	}
	
	// POST: Pipe for reading output from CGI script (stdout redirection)
	int cgi_pipe[2];
	if (pipe(cgi_pipe) == -1) {
		throw std::runtime_error("pipe");
	}
	
	pid_t pid = fork();
	if (pid < 0) {
		throw std::runtime_error("fork");
	}
	else if (pid == 0)  // child
	{
		printf("this is a POST child process\n");
		
		//*** POST request ***
		close(pipefd[1]); // Close the write end of the POST data pipe (we only need to read from it)
		dup2(pipefd[0], STDIN_FILENO); // Redirect stdin to read from the pipe (POST data)
		close(pipefd[0]); // Close the original read end after dup2
		
		close(cgi_pipe[0]); // Close the read end of the CGI output pipe (we only need to write to it)
		dup2(cgi_pipe[1], STDOUT_FILENO); // Redirect stdout to write to the pipe (CGI script output)
		close(cgi_pipe[1]); // Close the original write end after dup2
		
		if (execve(_path_to_script.c_str(), _argv, _env.getEnv()) == -1)
			printf("EXECVE FAILURE\n");
	}
	else // parent
	{		
		//*** POST request ***
		close(pipefd[0]);
		write(pipefd[1], _parsedRequest->getBody().c_str(), _parsedRequest->getBody().length());
		close(pipefd[1]);
		close(cgi_pipe[1]);
		char buf[1024];
		int nbytes = read(cgi_pipe[0], buf, sizeof(buf)); // don't forget to do it in a loop later
		buf[nbytes] = '\0';
		printf("buf: {%s}\n", buf);
		_response->setStatus(200, "OK");
		_response->addHeader("Content-Type", "text/html");
		_response->addHeader("Content-Length", sizeToString_CGI(nbytes));
		_response->setBody(buf);
		std::string str = _response->serialize();
		if (send(client_fd, str.c_str(), str.length(), 0) == -1)
			throw std::runtime_error(strerror(errno));
		close(cgi_pipe[0]);
		int status;
		waitpid(pid, &status, 0);
	}
}

CGIProcessor::~CGIProcessor() {}