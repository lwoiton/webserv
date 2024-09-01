/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwoiton <lwoiton@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 16:46:18 by lwoiton           #+#    #+#             */
/*   Updated: 2024/08/30 14:31:26 by lwoiton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHandler.hpp"

/* std::string	RequestHandler::findFile(const std::string& path)
{

}

bool	RequestHandler::fileExists(const std::string& path)
{

}

std::string	RequestHandler::readFile(const std::string& path)
{

}

std::string RequestHandler::generateHttpDate(void)
{
    time_t now = time(0);
    struct tm *gmt = gmtime(&now);
    char buffer[50];
    strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", gmt);
    return std::string(buffer);
}

void	RequestHandler::handleRequest(const Request& req, Response& res)
{
    if (req._version != "HTTP/1.1")
        throw std::runtime_error("invalid version")
    switch (req.__method)
    {
    case "GET":
        handleGET()
        break;
    case "POST":
        handlePOST()
        break;
    case "DELETE":
        handleDELETE()
        break;
    default:
        returnError()
        break;
    }
    // roll it up from the back
    // 1. check if version is compatible
    // 11 Check for the method
    // 2. check if requested file is availabel looking through all possible paths
    // 3. create body
    // 4. create header lines
    //      a. Date:
    //      b. Server:
    //      c. Last-Modified:
    //      d. ETag:
    //      e. Accept-Ranges: 
    //      f. Content-Length:
    //      g. Vary:
    //      h. Content-Type: text/html

} */
