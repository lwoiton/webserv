/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwoiton <lwoiton@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 14:15:39 by lwoiton           #+#    #+#             */
/*   Updated: 2024/09/01 15:12:14 by lwoiton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request(void)
{

};

static std::string	trim(const std::string& str)
{
	size_t	start = str.find_first_not_of(" \t\r\n");
	if (start == std::string::npos)
		return ("");
	size_t	end = str.find_last_not_of(" \t\r\n");
	return (str.substr(start, (end - start) + 1));
}

//     HTTP-message   = start-line
//                      *( header-field CRLF )
//                      CRLF
//                      [ message-body ]
//parse request line
	//devide line through Single Space into method, uri and version
	//check if method is valid, uri is valid and version is valid
	//method "GET" "POST" "DELETE"
	//uri origin-form absolute-form authority-form
	//version HTTP/1.1
//parse header
	// field-name : OWS field-value OWS CRLF (\r\n)
//parse body
void	Request::parse(const std::string& rawRequest)
{
	std::istringstream	stream(rawRequest);
	std::string			line;

	std::cout << "Raw request length: " << rawRequest.length() << std::endl;
    std::cout << "Raw request:\n" << rawRequest << std::endl;  // Print the entire raw request
	// Parse request line
	if (!std::getline(stream, line))
		throw std::runtime_error("Empty request");
	std::istringstream	startLine(line);
	if (!(startLine >> _method >> _uri >> _version))
		throw std::runtime_error("Invalid request line");
	// Parse headers
	while (std::getline(stream, line) && line != "\r" && line != "")
	{
		size_t colonPos = line.find(':');
		if (colonPos == std::string::npos)
			throw std::runtime_error("Invalid header format");
		std::string fieldName = trim(line.substr(0, colonPos));
		std::string fieldValue = trim(line.substr(colonPos + 1));
		if (fieldName.empty())
			throw std::runtime_error("Empty header field name");
		this->_header[fieldName] = fieldValue;
	}

	// Parse body efficiently
	std::string content_length_str = this->_header["Content-Length"];
	if (!content_length_str.empty())
	{
		std::istringstream iss(content_length_str);
		size_t content_length;
		if (!(iss >> content_length))
			throw std::runtime_error("Invalid Content-Length");
		this->_body.reserve(content_length);
		std::getline(stream, this->_body, '\0');
		if (this->_body.length() != content_length)
			throw std::runtime_error("Body length mismatch");
	}
	else
	{
		// If no Content-Length, read until end of stream
		std::ostringstream oss;
		oss << stream.rdbuf();
		this->_body = oss.str();
	}
};

void	Request::printRequest(void)
{
	std::cout << this->_method << " " << this->_uri << " " << this->_version << "\r\n";
	for (std::map<std::string, std::string>::const_iterator it = _header.begin(); it != _header.end(); ++it)
		std::cout << it->first << ": " << it->second  << "\r\n";
	std::cout << this->_body << std::endl;
}