/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwoiton <lwoiton@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:41:03 by lwoiton           #+#    #+#             */
/*   Updated: 2024/10/21 21:13:26 by lwoiton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef CONNECTION_HPP
# define CONNECTION_HPP

#include <vector>

#include "Logger.hpp"
#include "Request.hpp"
#include "Response.hpp"

enum State
{
	IDLE,
	RECV_DATA,
	PARSE_REQUEST
	PROCESS_REQUEST,
	WRITING_RESPONSE,
	SEND_DATA,
};

class Connection
{
	private:
		int						_sockfd;
		std::vector<uint8_t>    _readBuffer;
		std::vector<uint8_t>	_writeBuffer;
		State					_state;
		Request                 _req;
		Response                _res;
		// Add further information (remote IP remote port etc.)
	public:
				Connection(int sockfd);
				~Connection(void);
		ssize_t	receiveData(void);
		void	parseRequest(void);
		void	processRequest(void);
		void	generateResponse(void);
		ssize_t	sendData(void);
}

#endif