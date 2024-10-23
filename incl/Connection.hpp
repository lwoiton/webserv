/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwoiton <lwoiton@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:41:03 by lwoiton           #+#    #+#             */
/*   Updated: 2024/10/23 20:57:44 by lwoiton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef CONNECTION_HPP
# define CONNECTION_HPP

#include <vector>
# include <sys/socket.h>
# include <errno.h>
# include <unistd.h>

#include "Utils.hpp"
#include "Logger.hpp"
#include "Request.hpp"
#include "Response.hpp"

enum ConnectionState
{
    IDLE,
    RECV_DATA,
    PARSE_REQUEST,
    PROCESS_REQUEST,
    SEND_DATA
};

class Connection
{
	private:
							Connection(const Connection&);  // Prevent copying
		Connection&			operator=(const Connection&);
		int					_sockfd;
		std::vector<char>	_readBuffer;
		std::vector<char>	_writeBuffer;
		ConnectionState		_state;
		Request             _req;
		Response            _res;
		// Add further information (remote IP remote port etc.)
	public:
					Connection(int sockfd);
					~Connection(void);
		
		ssize_t		receiveData(void);
		void		parseRequest(void);
		void		processRequest(void);
		void		generateResponse(void);
		ssize_t		sendData(void);
};

#endif