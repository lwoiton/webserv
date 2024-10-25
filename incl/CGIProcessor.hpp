#ifndef CGIPROCESSOR_HPP
#define CGIPROCESSOR_HPP

#include <iostream>
#include <string>
#include "Request.hpp"
#include "Response.hpp"
#include "Environment.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cerrno>

class CGIProcessor {
    private:
        Request *_parsedRequest;
        Response *_response;
        Environment _env;
        std::string _path_to_script;
        void handleGETCGI(int client_fd, char * _argv[]);
        void handlePOSTCGI(int client_fd, char * _argv[]);
    public:
        CGIProcessor(Request *req, Response *res);
        ~CGIProcessor();
        void handleCGIRequest(int client_fd);
};

#endif