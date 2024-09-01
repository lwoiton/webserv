/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwoiton <lwoiton@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 10:22:04 by lwoiton           #+#    #+#             */
/*   Updated: 2024/08/19 14:41:13 by lwoiton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"

ConfigParser::ConfigParser(void)
{
    //Absolute Defualt specification
    this->config["listen"] = "8080";
}

ConfigParser::ConfigParser(const char* filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open config file");
    }

    std::string line;
    bool inServerBlock = false;

    while (std::getline(file, line)) {
        // Remove leading and trailing whitespace
        std::string::size_type start = line.find_first_not_of(" \t");
        std::string::size_type end = line.find_last_not_of(" \t");
        if (start != std::string::npos && end != std::string::npos) {
            line = line.substr(start, end - start + 1);
        } else {
            line.clear();
        }

        if (line.empty() || line[0] == '#') {
            continue;  // Skip empty lines and comments
        }

        if (line == "server {") {
            inServerBlock = true;
        } else if (line == "}") {
            inServerBlock = false;
        } else if (inServerBlock) {
            std::string::size_type pos = line.find(' ');
            if (pos != std::string::npos) {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);
                // Remove semicolon if present
                if (!value.empty() && value[value.length() - 1] == ';') {
                    value = value.substr(0, value.length() - 1);
                }
                this->config[key] = value;
            }
        }
    }

    // Ensure we have a listen directive
    if (config.find("listen") == config.end()) {
        config["listen"] = "8080";  // Default to 8080 if not specified
    }
}

std::string ConfigParser::getValue(const std::string& key) const
{
    std::map<std::string, std::string>::const_iterator it = config.find(key);
    if (it != config.end()) {
        return it->second;
    }
    return "";  // Return empty string if key not found
}

void ConfigParser::printConfig() const
{
    std::map<std::string, std::string>::const_iterator it;
    for (it = config.begin(); it != config.end(); ++it) {
        std::cout << it->first << ": " << it->second << std::endl;
    }
}