/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnurlybe <mnurlybe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 11:17:56 by lwoiton           #+#    #+#             */
/*   Updated: 2024/10/05 17:01:05 by mnurlybe         ###   ########.fr       */
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

#define NUM_OF_EVENTS 100

class Server
{
	private:
		SimpleSocket *					_serverSocket;
		struct epoll_event				_events[NUM_OF_EVENTS];
		int								_epoll_fd;
		ConfigParser					_config;
	public:
				Server(void);
				Server(const char* config_filename);
		void	run();
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
