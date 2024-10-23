/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwoiton <lwoiton@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 11:17:56 by lwoiton           #+#    #+#             */
/*   Updated: 2024/10/22 15:06:35 by lwoiton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef SERVER_HPP
# define SERVER_HPP

// 1. C++ Standard Library headers
# include <iostream>
# include <string>
# include <vector>
# include <map>
# include <fstream>
# include <sstream>

// 2. C headers through C++ wrappers
# include <cstring>
# include <cstdlib>
# include <cerrno>

// 3. System/Socket headers wrapped in extern "C"
# include <sys/types.h>
# include <sys/socket.h>
# include <sys/epoll.h>
# include <sys/wait.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <netinet/in.h>
# include <unistd.h>
# include <fcntl.h>
# include <signal.h>

// 4. Your project headers
# include "Utils.hpp"
# include "Connection.hpp"
# include "SimpleSocket.hpp"
# include "ConfigParser.hpp"
# include "Request.hpp"
# include "Response.hpp"
# include "Environment.hpp"
# include "Logger.hpp"

// Rest of your Server class definition...

#define MAX_NUM_OF_EVENTS 100

extern volatile sig_atomic_t g_signal_received;

void signalHandler(int __attribute__((unused)) signum);

class Server
{
	private:
		SimpleSocket 					*_serverSocket;
		struct epoll_event				_events[MAX_NUM_OF_EVENTS];
		int								_epoll_fd;
		std::map<int, Connection*>		_conns;
		ConfigParser					_config;
	public:
				Server(void);
				Server(const char* config_filename);
		void	run();
		void	shutdown(int events_num);
		void	epCTL(int fd, int _op, int event_flag);
		void	handleNewConnection();
		void	handleExistingConnection(struct epoll_event& ee);
		void	closeConnection(int fd);

		// CGI handling member functions
		// handleCGIRequest will be called when a request is detected as a CGI request
		// it will check what request method is used, and based on that it will create the correct environment variables array and args
		// and call the correct function to handle the request
		
		void	handleCGIRequest(int client_fd, Request &req, Response &res);
		void	handlePOSTCGI(int client_fd, Request &req, Response &res, Environment &env, std::string _path_to_script, char * _argv[]);
		void	handleGETCGI(int client_fd, Response &res, Environment &env, std::string _path_to_script, char * _argv[]);
		
};

#endif
