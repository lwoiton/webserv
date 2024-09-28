/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julienmoigno <julienmoigno@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 14:10:31 by lwoiton           #+#    #+#             */
/*   Updated: 2024/09/28 20:37:00 by julienmoign      ###   ########.fr       */
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
}

Server::Server(const char* config_filename)
{
	this->_config = ConfigParser(config_filename);
	this->_serverSocket = new SimpleSocket(NULL,  this->_config.getValue("listen").c_str());
    this->_epoll_fd = epoll_create(2);
    if (this->_epoll_fd < 0) {
        throw std::runtime_error(strerror(errno));
    }
    addToEpoll(this->_serverSocket->get_socket(), EPOLLIN, EPOLL_CTL_ADD);
}


void    Server::addToEpoll(int new_fd, int event_flag, int _op)
{
    struct epoll_event  event;

    event.events = event_flag;
    event.data.fd = new_fd;
    epoll_ctl(this->_epoll_fd, _op, new_fd, &event);
}


void    Server::run(void)
{
    while (true)
    {
        int events_num = epoll_wait(this->_epoll_fd, this->_events, NUM_OF_EVENTS, -1);
        if (events_num < 0)
            throw std::runtime_error(strerror(errno));
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
        addToEpoll(new_sfd, EPOLLIN, EPOLL_CTL_ADD);
		char remoteIP[INET6_ADDRSTRLEN];
		std::cout << "============================" << std::endl;
		std::cout << "server: new connection from " << inet_ntop(remoteaddr.ss_family, get_in_addr((struct sockaddr*)&remoteaddr), remoteIP, INET6_ADDRSTRLEN) << " on socket " << new_sfd << '\n' ;
		std::cout << "============================" << std::endl;
	}
}

std::string sizeToString(size_t value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

void	Server::handleExistingConnection(int epfd_index)
{
	char	buf[10000];
	int	nbytes = recv(this->_events[epfd_index].data.fd, buf, sizeof(buf), 0);

	if (nbytes <= 0)
	{
		if (nbytes == 0)
			std::cout << "server: socket " << this->_events[epfd_index].data.fd << " hung up!\n";
		else
			throw std::runtime_error("recv");
		
        addToEpoll(this->_events[epfd_index].data.fd, 0, EPOLL_CTL_DEL);
        close(this->_events[epfd_index].data.fd);
	}
	else
	{
		Request		req;
		Response	res;

		req.parse(buf);
		std::cout << "=============printRequest===============" << std::endl;
		req.printRequest();
		std::cout << "========================================" << std::endl;

		/* implement CGI here */
		// 1. check if the request is a CGI request
		std::cout << "~~~~~~ isCGI: " << req.isCGI() << std::endl;
		// 2. if it is a CGI request, execute the CGI script
		// 3. if it is not a CGI request, handle the request
		
		if (req.isCGI())
		{
			// execute CGI script
			// CGI cgi;
			Environment env(3);
			std::string _python = "/usr/bin/python3";
			std::string _path_to_script = "./var/www/cgi-bin/hello.py";
			printf("HELLO\n");
			char * _argv[] = {const_cast<char*>(_python.c_str()), const_cast<char*>(_path_to_script.c_str()), NULL};
			env.AddEnvVar(0, "REQUEST_METHOD", "GET");
			env.AddEnvVar(1, "SCRIPT_FILENAME", _path_to_script.c_str());
			
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
				printf("this is a child process\n");
				close(pipefd[0]);
				dup2(pipefd[1], STDOUT_FILENO);
				close(pipefd[1]);
				if (execve(_python.c_str(), _argv, env.getEnv()) == -1)
					throw std::runtime_error("execve");
			}
			else // parent
			{
				close(pipefd[1]);
				char buf[10000];
				int nbytes = read(pipefd[0], buf, sizeof(buf)); // don't forget to do it in a loop later
				printf("nbytes: %d\n", nbytes);
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
				if (send(this->_events[epfd_index].data.fd, str.c_str(), str.length(), 0) == -1)
					throw std::runtime_error(strerror(errno));
				close(pipefd[0]);
				int status;
				waitpid(pid, &status, 0);
			}		
		}
		else
		{
			// handle the request
			res.setStatus(200, "OK");
			res.addHeader("Content-Type", "text/html");
			res.setBody(readFile("./public/index.html"));
			res.addHeader("Content-Length", sizeToString(res.getBody().length()));
			std::string str = res.serialize();
			if (send(this->_events[epfd_index].data.fd, str.c_str(), str.length(), 0) == -1)
				throw std::runtime_error(strerror(errno));
		}
	}
}


