/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julienmoigno <julienmoigno@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 11:17:56 by lwoiton           #+#    #+#             */
/*   Updated: 2024/09/28 17:30:59 by julienmoign      ###   ########.fr       */
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
#include "Cgi.hpp"
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
};

#endif
