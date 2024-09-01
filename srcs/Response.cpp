/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwoiton <lwoiton@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 16:32:16 by lwoiton           #+#    #+#             */
/*   Updated: 2024/08/30 22:24:07 by lwoiton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"


Response::Response(void)
{

}

void		Response::setStatus(int code, const std::string& message)
{
    this->_version = "HTTP/1.1";
    this->_statusCode = code;
    this->_statusMessage = message;
}

void		Response::addHeader(const std::string& key, const std::string& value)
{
    this->_header[key] = value;
}

void		Response::setBody(const std::string& content)
{
    this->_body = content;
}

std::string	Response::getBody(void)
{
    return (this->_body);
}

std::string	Response::serialize(void)
{
	std::ostringstream oss;

	oss << this->_version << " " << this->_statusCode << " " << this->_statusMessage << "\r\n";
    for (std::map<std::string, std::string>::iterator it = this->_header.begin(); it != this->_header.end(); it++)
	{
        oss << it->first << ": " << it->second << "\r\n";
	}
	oss << "\r\n";
	oss << this->_body;
    return (oss.str());
}