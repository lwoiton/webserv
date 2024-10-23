/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwoiton <lwoiton@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:51:45 by lwoiton           #+#    #+#             */
/*   Updated: 2024/10/22 15:14:56 by lwoiton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"

Connection::Connection(int sockfd) : _sockfd(sockfd), _state(IDLE)
{
    LOG_INFO("Connection established.");
    //Add more information
}

Connection::~Connection(void)
{
    LOG_INFO("Connection closed!");
}

ssize_t	Connection::receiveData(void)
{
    char    buf[4096];
    ssize_t nbytes;
    
    this->_state = RECV_DATA;
    while (true)
    {
        nbytes = recv(this->_sockfd, buf, sizeof(buf), 0);
        if (nbytes > 0)
            this->_readBuffer.insert(this->_readBuffer.end(), buf, buf + nbytes);
        else if (nbytes == 0)
            return (0); // Connection closed
        else
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
				this->_readBuffer[this->_readBuffer.size()] = '\0';
                LOG_INFO("Request successfully received.");
                return (this->_readBuffer.size()); //nothing left to receive
            }
            else
            {
                LOG_ERROR("recv()");
                return (-1); // Error occoured
            }
        }
    }
}

ssize_t	Connection::sendData(void)
{
    this->_state = SEND_DATA;
    while (!this->_writeBuffer.empty())
    {
        ssize_t sent = send(this->_sockfd, &this->_writeBuffer[0], this->_writeBuffer.size(), 0);
        if (sent > 0)
            this->_writeBuffer.erase(this->_writeBuffer.begin(), this->_writeBuffer.end() + sent);
        else if (sent == -1)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                LOG_WARNING("send would block: still something remaining, try again later");
                return (this->_writeBuffer.size());
            }
            LOG_ERROR("send()");
            return (-1); // Error occoured
        }
    }
    this->_state = IDLE;
    LOG_INFO("Response sent succesfully");
    return (0); // All data send
}

void	Connection::parseRequest(void)
{
    this->_state = PARSE_REQUEST;
    this->_req = Request();
    std::string rawRequest;
    if (!this->_readBuffer.empty()) {
        rawRequest.assign(this->_readBuffer.begin(), this->_readBuffer.end());
    }
    this->_req.parse(rawRequest);
    this->_req.printRequest();
    LOG_INFO("Request parsed succesfully!");
    this->_state = PROCESS_REQUEST;
    this->processRequest();
}

void	Connection::processRequest(void)
{
    LOG_INFO("Processsing.....");
    if (this->_req.isCGI())
    {
        LOG_INFO("CGI request detected");
        //handleCGIRequest();
    }
    else
    {
        LOG_INFO("Static request detected");
        this->generateResponse();
    }
    this->_state = SEND_DATA;
    this->generateResponse();
    this->sendData();
}

void	Connection::generateResponse(void)
{
    LOG_INFO("Generating HTTP response");
    this->_res.setStatus(200, "OK");
    this->_res.addHeader("Content-Type", "text/html");
    this->_res.setBody(readFile("./public/index.html"));
    this->_res.addHeader("Content-Length", sizeToString(_res.getBody().length()));
	std::string response = this->_res.serialize();
    this->_writeBuffer.insert(this->_writeBuffer.end(), response.begin(), response.end());
}