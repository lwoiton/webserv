/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julienmoigno <julienmoigno@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 13:45:16 by lwoiton           #+#    #+#             */
/*   Updated: 2024/09/28 14:58:59 by julienmoign      ###   ########.fr       */
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
};

#endif