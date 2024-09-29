/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnurlybe <mnurlybe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 13:45:16 by lwoiton           #+#    #+#             */
/*   Updated: 2024/09/29 15:04:57 by mnurlybe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <string>
#include <map>
#include <iostream>
#include <sstream>

class Request
{
	private:
		std::string							_method;
		std::string							_uri;
		std::string							_version;
		std::map<std::string, std::string>	_header;
		std::string							_body;
	public:
				Request(void);
		void	parse(const std::string& rawRequest);
		void	printRequest(void);
		// check if the request is a CGI request
		int		isCGI(void);
		std::string getBody() const;
};

#endif