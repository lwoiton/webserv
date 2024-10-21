/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwoiton <lwoiton@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 10:56:44 by lwoiton           #+#    #+#             */
/*   Updated: 2024/10/20 22:51:52 by lwoiton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>

enum LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

class Logger {
private:
    static Logger* instance;
    std::ofstream logFile;
    LogLevel currentLevel;

    Logger();
    Logger(const Logger&);
    Logger& operator=(const Logger&);

    std::string getTimestamp();
    std::string getLevelString(LogLevel level);

public:
    static Logger* getInstance();
    void setLogLevel(LogLevel level);
    void log(LogLevel level, const std::string& message, const char* file = NULL, int line = -1);

    ~Logger();
};

#define LOG_DEBUG(message) Logger::getInstance()->log(DEBUG, message, __FILE__, __LINE__)
#define LOG_INFO(message) Logger::getInstance()->log(INFO, message)
#define LOG_WARNING(message) Logger::getInstance()->log(WARNING, message, __FILE__, __LINE__)
#define LOG_ERROR(message) Logger::getInstance()->log(ERROR, message, __FILE__, __LINE__)

#endif // LOGGER_HPP