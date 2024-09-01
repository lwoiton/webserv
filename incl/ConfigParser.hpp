/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwoiton <lwoiton@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 09:04:17 by lwoiton           #+#    #+#             */
/*   Updated: 2024/08/29 13:23:03 by lwoiton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef CONFIGPARSER_HPP
# define CONFIGPARSER_HPP

#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class ConfigParser
{
	private:
		std::map<std::string, std::string> config;
	public:
					ConfigParser(void);
					ConfigParser(const char* filename);
		std::string	getValue(const std::string& key) const;
		void		printConfig(void) const;
};

#endif