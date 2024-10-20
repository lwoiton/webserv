/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwoiton <lwoiton@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 11:17:56 by lwoiton           #+#    #+#             */
/*   Updated: 2024/10/20 14:45:19 by lwoiton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <vector>
#include <exception>
#include <stdexcept>
#include <cstring>
#include <cerrno>
#include <unistd.h>


#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <fcntl.h>

#include <arpa/inet.h> // for inet_ntop()

#include "Logger.hpp"
#include "ConfigParser.hpp"
#include "SimpleSocket.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Environment.hpp"
//#include "RequestHandler.hpp"

#include <fstream> //TO DELETE AFTER successful routing and Repsonse implementations

#define MAX_NUM_OF_EVENTS 100

extern int g_signal_received;

class Server
{
	private:
		SimpleSocket 					*_serverSocket;
		struct epoll_event				_events[MAX_NUM_OF_EVENTS];
		int								_epoll_fd;
		ConfigParser					_config;
	public:
				Server(void);
				Server(const char* config_filename);
		void	run();
		void	shutdown(int events_num);
		void	initializeSockets(void);
		void	addToEpoll(int new_fd, int event_flag, int _op);
		void	handleNewConnection();
		void	handleExistingConnection(int index_pfds);

		// CGI handling member functions
		// handleCGIRequest will be called when a request is detected as a CGI request
		// it will check what request method is used, and based on that it will create the correct environment variables array and args
		// and call the correct function to handle the request
		
		void	handleCGIRequest(int client_fd, Request &req, Response &res);
		void	handlePOSTCGI(int client_fd, Request &req, Response &res, Environment &env, std::string _path_to_script, char * _argv[]);
		void	handleGETCGI(int client_fd, Response &res, Environment &env, std::string _path_to_script, char * _argv[]);
		
};

#endif
