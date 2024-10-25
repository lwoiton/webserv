/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julienmoigno <julienmoigno@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 14:10:31 by lwoiton           #+#    #+#             */
/*   Updated: 2024/10/25 19:39:09 by julienmoign      ###   ########.fr       */
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
	this->_userDatabase = UserDatabase();
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
	this->_userDatabase = UserDatabase();
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

// std::string readFile(const std::string& filename)
// {
// 	std::ifstream file(filename.c_str());
// 	if (!file.is_open()) {
// 		return "<html><body><h1>404 Not Found</h1></body></html>";
// 	}
// 	return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
// }

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

		buf[nbytes] = '\0'; // null-terminate the received data as without it there is a cached data;
		req.parse(buf);
		std::cout << "=============printRequest===============" << std::endl;
		req.printRequest();
		std::cout << "========================================" << std::endl;

		std::cout << "~~~~~~ isCGI: " << req.isCGI() << std::endl;
		if (req.isCGI())
		{
			CGIProcessor CGI_P(&req, &res);
			CGI_P.handleCGIRequest(this->_events[epfd_index].data.fd);
		}
		else
		{
			// // handle the request
			// res.setStatus(200, "OK");
			// res.addHeader("Content-Type", "text/html");
			// // res.setBody(readFile("./public/index.html"));
			// // res.setBody(readFile("./public/form.html"));
			// res.setBody(readFile("./public/delete_user.html"));
			// res.addHeader("Content-Length", sizeToString(res.getBody().length()));
			// std::string str = res.serialize();
			// if (send(this->_events[epfd_index].data.fd, str.c_str(), str.length(), 0) == -1)
			// 	throw std::runtime_error(strerror(errno));

			RequestProcessor rp(&req, &res, &_userDatabase);
			rp.processRequest();
			std::string str = res.serialize();
			if (send(this->_events[epfd_index].data.fd, str.c_str(), str.length(), 0) == -1)
				throw std::runtime_error(strerror(errno));
		}
	}
}
