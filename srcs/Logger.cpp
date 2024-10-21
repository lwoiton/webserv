/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwoiton <lwoiton@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 11:15:25 by lwoiton           #+#    #+#             */
/*   Updated: 2024/10/20 23:10:02 by lwoiton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Logger.hpp"

Logger* Logger::instance = NULL;

Logger::Logger() : currentLevel(INFO) {
    logFile.open("log.txt", std::ios::app);
}

Logger* Logger::getInstance() {
    if (instance == NULL) {
        instance = new Logger();
    }
    return instance;
}

void Logger::setLogLevel(LogLevel level) {
    currentLevel = level;
}

std::string Logger::getTimestamp() {
    time_t now = time(0);
    struct tm* timeinfo = localtime(&now);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %X", timeinfo);
    return std::string(buffer);
}

std::string Logger::getLevelString(LogLevel level) {
    switch(level) {
        case DEBUG: return "DEBUG";
        case INFO: return "INFO";
        case WARNING: return "WARNING";
        case ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

void Logger::log(LogLevel level, const std::string& message, const char* file, int line) {
    if (level < currentLevel) return;

    std::string output = "[" + getTimestamp() + "]" + " [" + getLevelString(level) + "] " + message;
    
    if (file != NULL && line != -1 && (level == WARNING || level == ERROR)) {
        std::stringstream ss;
        ss << " (File: " << file << ", Line: " << line << ")";
        output += ss.str();
    }
    logFile << output << std::endl;
	if (level == ERROR || level == WARNING)
		std::cerr << output << std::endl;
	else
    	std::cout << output << std::endl;
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}