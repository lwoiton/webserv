#ifndef REQUESTPROCESSOR_HPP
#define REQUESTPROCESSOR_HPP

#include <iostream>
#include <string>
#include <fstream>
#include "Request.hpp"
#include "Response.hpp"
#include "DataBase.hpp"

class RequestProcessor {
    private:
        std::string _request;
        Request *_parsedRequest;
        Response *_response;
        UserDatabase *_usersDB;
        std::string _endpoint;
        std::map <std::string, std::string> _queryParams;
        void handleGETRequest();
        void parseURI(std::string uri);

    public:
        RequestProcessor(Request *req, Response *res, UserDatabase *userDB);
        ~RequestProcessor();

        void processRequest();
};

#endif