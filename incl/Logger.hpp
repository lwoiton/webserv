/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwoiton <lwoiton@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 10:56:44 by lwoiton           #+#    #+#             */
/*   Updated: 2024/08/30 14:33:10 by lwoiton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef LOGGER_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include <map>

/*
	To make the Logger available by all other classes and functions, the singleton pattern was applied.

	Steps to Implement Singleton Class in C++:
    1. Make all the constructors of the class private.
    2. Delete the copy constructor of the class.
    3. Make a private static pointer that can point to the same class object (singleton class).
    4. Make a public static method that returns the pointer to the same class object (singleton class).
*/
class Logger
{
	public:
		enum LogLevel
		{
			DEBUG,
			INFO,
			WARNING,
			ERROR
		};
		static Logger&	getInstance();
		void			setLogFile(const std::string& log_filename);
		void			setLogLevel(LogLevel level);
		void			log(LogLevel level, const std::string& message);
	private:
										Logger();
										~Logger();
										Logger(const Logger& src); //needed to prevent copying to comply with "Singleton Pattern"
		Logger&							operator=(const Logger& src); //needed to prevent copying to comply with "Singleton Pattern"
		std::ofstream					_logFile;
		static LogLevel					_currentLevel;
		std::map<LogLevel, std::string>	_levelStrings;
		std::string						getCurrentTimestamp() const;
		void							writeLog(const std::string& message);
};

//Simplify the acces to the Logging on specififc levels
#define LOG_DEBUG(message)		Logger::getInstance().log(Logger::DEBUG, message);
#define LOG_INFO(message)		Logger::getInstance().log(Logger::INFO, message);
#define LOG_WARNING(message)	Logger::getInstance().log(Logger::WARNING, message);
#define LOG_ERROR(message)		Logger::getInstance().log(Logger::ERROR, message);

#endif 