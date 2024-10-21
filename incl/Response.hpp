/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwoiton <lwoiton@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 14:08:05 by lwoiton           #+#    #+#             */
/*   Updated: 2024/10/20 22:51:58 by lwoiton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once
#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <string>
#include <map>
#include <sstream> //for std::ostringstream

class Response
{
	private:
		std::string							_version;
		int									_statusCode;
		std::string							_statusMessage;
		std::map<std::string, std::string>	_header;
		std::string							_body;
	public:
					Response(void);
		void		setStatus(int code, const std::string& message);
		void		addHeader(const std::string& key, const std::string& value);
		void		setBody(const std::string& content);
		std::string	getBody(void);
		std::string	serialize(void);
};

#endif