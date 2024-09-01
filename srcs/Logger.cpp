/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwoiton <lwoiton@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 11:15:25 by lwoiton           #+#    #+#             */
/*   Updated: 2024/08/30 14:34:47 by lwoiton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Logger.hpp"

Logger::LogLevel Logger::_currentLevel = 
#ifdef DEBUG
    Logger::DEBUG
#else
    Logger::INFO
#endif
;

Logger&	Logger::getInstance(void)
{
	static Logger	instance;
	return (instance);
}

Logger::Logger()
{
	this->_levelStrings[DEBUG] = "DEBUG";
	this->_levelStrings[INFO] = "INFO";
	this->_levelStrings[WARNING] = "WARNING";
	this->_levelStrings[ERROR] = "ERROR";
}

Logger::~Logger()
{
    if (this->_logFile.is_open())
        this->_logFile.close();
}

void	Logger::setLogFile(const std::string& log_filename)
{
	if (this->_logFile.is_open())
		this->_logFile.close();
	this->_logFile.open(log_filename.c_str(), std::ios::app);
}

void	Logger::setLogLevel(LogLevel level)
{
	this->_currentLevel = level;
}

void	Logger::log(LogLevel level, const std::string& message)
{
	if (level >= this->_currentLevel)
	{
		std::string	logMessage = getCurrentTimestamp() + " [" + _levelStrings[level] + "]: " + message;
		writeLog(logMessage);
	}
}

std::string	Logger::getCurrentTimestamp() const
{
	time_t	now = time(0);
	struct tm	tstruct;
	char	buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
	return (buf);	
}

void	Logger::writeLog(const std::string& message)
{
	if (this->_logFile.is_open())
		this->_logFile << message << std::endl;
	std::cout << message << std::endl;
}