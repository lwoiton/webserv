/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julienmoigno <julienmoigno@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 13:45:16 by lwoiton           #+#    #+#             */
/*   Updated: 2024/10/03 18:45:27 by julienmoign      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <string>
#include <map>
#include <iostream>
#include <sstream>
#include <cstdio>

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
		
		// CGI handling and getter -> added by Madina
		int		isCGI(void);
		std::string getBody() const;
		std::string getMethod() const;
		std::string getUri() const;
		std::string getVersion() const;
		std::string getHeaderValue(const std::string& key) const;
};

#endif