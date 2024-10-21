/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwoiton <lwoiton@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 14:10:31 by lwoiton           #+#    #+#             */
/*   Updated: 2024/10/21 21:35:43 by lwoiton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(void)
{
	this->_config = ConfigParser();
	this->_serverSocket = new SimpleSocket(NULL,  this->_config.getValue("listen").c_str());
    this->_epoll_fd = epoll_create(2); //maybe change to epoll_create1()
    if (this->_epoll_fd < 0) {
        throw std::runtime_error(strerror(errno));
    }
	epCTL(this->_serverSocket->get_socket(), EPOLL_CTL_ADD, EPOLLIN);
}

Server::Server(const char* config_filename)
{
	this->_config = ConfigParser(config_filename);
	this->_serverSocket = new SimpleSocket(NULL,  this->_config.getValue("listen").c_str());
    this->_epoll_fd = epoll_create(2);
    if (this->_epoll_fd < 0) {
        throw std::runtime_error(strerror(errno));
    }
	epCTL(this->_serverSocket->get_socket(), EPOLL_CTL_ADD, EPOLLIN);
}	


void    Server::epCTL(int fd, int _op, int event_flag)
{
    struct epoll_event  event;

    event.events = event_flag;
    event.data.fd = fd;
    epoll_ctl(this->_epoll_fd, _op, fd, &event);
}


void    Server::run(void)
{
	int events_num = 0;
	while (!g_signal_received)
	{
		events_num = epoll_wait(this->_epoll_fd, this->_events, MAX_NUM_OF_EVENTS, -1);
		if (events_num < 0)
		{
			if (errno == EINTR && g_signal_received)
				break;
			else
			{
				throw std::runtime_error(strerror(errno));
			}
		}
		for (int i = 0; i < events_num; ++i)
		{
			if (this->_events[i].data.fd == this->_serverSocket->get_socket())
			{
				handleNewConnection();
			}
			else
			{
				handleExistingConnection(i);
			}
		}
	}
	this->shutdown(events_num);
}

void Server::shutdown(int events_num)
{
    int index = 0;
    while (events_num-- > 0)
    {
        if (close(this->_events[index++].data.fd) < 0) {
			LOG_ERROR("Error closing fd: " + std::string(strerror(errno)));
			throw std::runtime_error("Error closing fd");
        }
    }
    if (close(this->_epoll_fd) < 0) {
		LOG_ERROR("Error closing epoll fd: " + std::string(strerror(errno)));
		throw std::runtime_error("Error closing epoll fd");
    }
    if (close(this->_serverSocket->get_socket()) < 0) {
		LOG_ERROR("Error closing server socket: " + std::string(strerror(errno)));
		throw std::runtime_error("Error closing server socket");
    }
    delete this->_serverSocket;
    LOG_INFO("Server shutdown");
    exit(EXIT_SUCCESS);  // Use EXIT_SUCCESS instead of EXIT_FAILURE if this is a normal shutdown
}

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

std::string readFile(const std::string& filename)
{
	std::ifstream file(filename.c_str());
	if (!file.is_open()) {
		return "<html><body><h1>404 Not Found</h1></body></html>";
	}
	return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

void	Server::handleNewConnection(void)
{
	int	new_sfd;
	struct sockaddr_storage remoteaddr;
	socklen_t	addrlen;
	
	addrlen = sizeof(remoteaddr);
	
	new_sfd = accept(this->_serverSocket->get_socket(), (struct sockaddr *)&remoteaddr, &addrlen);
	if (new_sfd >= 0)
	{
		int flags = fcntl(new_sfd, F_GETFL, 0);
	    fcntl(new_sfd, F_SETFL, flags | O_NONBLOCK);
		epCTL(new_sfd, EPOLL_CTL_ADD, EPOLLIN);
		this->_conns[new_sfd] = Connection(new_sfd);
		char remoteIP[INET6_ADDRSTRLEN];
		std::cout << "============================" << std::endl;
		LOG_INFO("New connection from " + std::string(inet_ntop(remoteaddr.ss_family, get_in_addr((struct sockaddr*)&remoteaddr), remoteIP, INET6_ADDRSTRLEN)) + " on socket " + intToString(new_sfd));
		std::cout << "============================" << std::endl;
	}
}

std::string sizeToString(size_t value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}


// Temporary buffer
// load into proper buffer

// parse the proper buffer once the request is complete


void	Server::handleExistingConnection(int epfd_index)
{
	int	fd;
	
	fd = this->_events[epfd_index].data.fd;
	if (this->_events[epfd_index].events & EPOLLIN)
	{
		receiveData(epfd_index);
	}
	else if (this->_events[epfd_index].events & EPOLLOUT)
	{
		sendData(epfd_index);
	}
	else
	{
		handleError(epfd_index);
	}
}
	
/* 	else
	{
		Request		req;
		Response	res;

		buf[nbytes] = '\0'; // null-terminate the received data as without it there is a cached data;
		req.parse(buf);
		std::cout << "=============printRequest===============" << std::endl;
		req.printRequest();
		std::cout << "========================================" << std::endl;

		std::cout << "~~~~~~ isCGI: " << req.isCGI() << std::endl;
		if (req.isCGI())
		{
			handleCGIRequest(this->_events[epfd_index].data.fd, req, res);
		}
		else
		{
			// handle the request
			res.setStatus(200, "OK");
			res.addHeader("Content-Type", "text/html");
			// res.setBody(readFile("./public/index.html"));
			res.setBody(readFile("./public/form.html"));
			// res.setBody(readFile("./public/upload_file.html"));
			res.addHeader("Content-Length", sizeToString(res.getBody().length()));
			std::string str = res.serialize();
			if (send(this->_events[epfd_index].data.fd, str.c_str(), str.length(), 0) == -1)
				throw std::runtime_error(strerror(errno));
		}
	}
} */


void Server::handleCGIRequest(int client_fd, Request &req, Response &res)
{
			printf("******************2**********************\n\n");
			std::cout << "Print req.getBody()" << std::endl;
			std::cout << req.getBody() << std::endl;
			std::cout << "Print req.getBody().length()" << std::endl;
			std::cout << req.getBody().length() << std::endl;
			printf("****************************************\n\n");

			Environment env(9);
			std::string _path_to_script = "./var/www" + req.getUri();
			
			env.createEnv(req, _path_to_script);
			char * _argv[] = {const_cast<char*>(_path_to_script.c_str()), NULL}; 

			if (req.getMethod() == "GET") {
				handleGETCGI(client_fd, res, env, _path_to_script, _argv);
			}
			else if (req.getMethod() == "POST") {
				handlePOSTCGI(client_fd, req, res, env, _path_to_script, _argv);	
			}
	
}

void Server::handleGETCGI(int client_fd, Response &res, Environment &env, std::string _path_to_script, char * _argv[])
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
				if (execve(_path_to_script.c_str(), _argv, env.getEnv()) == -1)
					LOG_ERROR("EXECVE FAILURE");
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
				res.setStatus(200, "OK");
				res.addHeader("Content-Type", "text/html");
				res.addHeader("Content-Length", sizeToString(nbytes));
				res.setBody(buf);
				std::string str = res.serialize();
				if (send(client_fd, str.c_str(), str.length(), 0) == -1)
					throw std::runtime_error(strerror(errno));
				close(pipefd[0]);
				int status;
				waitpid(pid, &status, 0);
			}
}

void Server::handlePOSTCGI(int client_fd, Request &req, Response &res, Environment &env, std::string _path_to_script, char * _argv[])
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
		
		if (execve(_path_to_script.c_str(), _argv, env.getEnv()) == -1)
			LOG_ERROR("EXECVE FAILURE");
	}
	else // parent
	{		
		//*** POST request ***
		close(pipefd[0]);
		write(pipefd[1], req.getBody().c_str(), req.getBody().length());
		close(pipefd[1]);
		close(cgi_pipe[1]);
		char buf[1024];
		int nbytes = read(cgi_pipe[0], buf, sizeof(buf)); // don't forget to do it in a loop later
		buf[nbytes] = '\0';
		printf("buf: {%s}\n", buf);
		res.setStatus(200, "OK");
		res.addHeader("Content-Type", "text/html");
		res.addHeader("Content-Length", sizeToString(nbytes));
		res.setBody(buf);
		std::string str = res.serialize();
		if (send(client_fd, str.c_str(), str.length(), 0) == -1)
			throw std::runtime_error(strerror(errno));
		close(cgi_pipe[0]);
		int status;
		waitpid(pid, &status, 0);
	}
}

