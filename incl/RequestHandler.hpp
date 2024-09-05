/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwoiton <lwoiton@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 16:40:22 by lwoiton           #+#    #+#             */
/*   Updated: 2024/08/30 14:31:25 by lwoiton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef REQUESTHANDLER_HPP
# define REQUESTHANDLER_HPP

#include <ctime>
#include <iostream>
#include <iomanip>

#include "Request.hpp"
#include "Response.hpp"

/*
 * 1. handle(const Request& request): This is the main method that all derived classes must implement. It takes a Request object and returns a Response object.
 * 2. canHandle(const Request& request) const: This method allows a handler to determine if it can process a given request. This can be useful for routing decisions.
 * 3. configure(const Configuration& config): This method allows each handler to be configured with server-wide or route-specific settings.
 * 4. createResponse() and createErrorResponse(): These are utility methods to help create standard and error responses. They're declared protected so that derived classes can use them.
 * 5. log(const std::string& message) const: A simple logging method that derived classes can use.
*/
/* class RequestHandler {
public:
    virtual ~RequestHandler() {}
    virtual Response handle(const Request& request) = 0;
    virtual bool canHandle(const Request& request) const = 0;
   //virtual void configure(const Configuration& config) = 0;
protected:
    virtual Response createResponse(int statusCode, const std::string& content) const;
    virtual Response createErrorResponse(int statusCode, const std::string& message) const;
    virtual void log(const std::string& message) const;
};

class StaticFileHandler : public RequestHandler
{

};

class CGIHandler : public RequestHandler
{

}; */


#endif