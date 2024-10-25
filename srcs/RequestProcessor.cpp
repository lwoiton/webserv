#include "RequestProcessor.hpp"

/* Constructor */
RequestProcessor::RequestProcessor(Request *req, Response *res, UserDatabase *userDB) : _parsedRequest(req), _response(res), _usersDB(userDB) {
    this->_request = req->getMethod();
    parseURI(req->getUri());
}

void RequestProcessor::processRequest() {
    if (this->_request == "GET") {
        this->handleGETRequest();
    } else {
        _response->setStatus(405, "Method Not Allowed");
    }
}

std::string readFile(const std::string& filename)
{
	std::ifstream file(filename.c_str());
	if (!file.is_open()) {
		return "<html><body><h1>404 Not Found</h1></body></html>";
	}
	return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

std::string _sizeToString(size_t value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

void RequestProcessor::handleGETRequest() {
    std::string url = _parsedRequest->getUri();
    std::cout << "GET request for: " << url << std::endl;
    std::cout << "{" << url << "}" << std::endl;
    std::cout << "{" << this->_endpoint << "}" << std::endl;
    std::cout << "{" << this->_queryParams["username"] << "}" << std::endl;

    if (this->_endpoint == "/") {
        _response->setStatus(200, "OK");
        _response->addHeader("Content-Type", "text/html");
        _response->setBody(readFile("./public/index.html"));
        _response->addHeader("Content-Length", _sizeToString(_response->getBody().length()));
        std::cout << "Request Processor Class: " << std::endl;
        std::cout << "{" << _response->getBody() << "}" << std::endl;
    }
    else if (this->_endpoint == "/users/get_user") {
        std::string res = _usersDB->printUserDetails_html(this->_queryParams["username"]);
        _response->setStatus(200, "OK");
        _response->addHeader("Content-Type", "text/html");
        _response->addHeader("Content-Length", _sizeToString(res.length()));
        _response->setBody(res);
    } else {
        _response->setStatus(404, "Not Found");
    }
}

void RequestProcessor::parseURI(std::string _uri) {
    std::string uri = _uri;
    size_t pos = uri.find("?");
    if (pos != std::string::npos) {
        this->_endpoint = uri.substr(0, pos);
        std::string query = uri.substr(pos + 1);
        std::string key, value;
        std::string::size_type i = 0;
        while (i < query.length()) {
            key.clear();
            value.clear();
            while (i < query.length() && query[i] != '=') {
                key += query[i];
                i++;
            }
            i++;
            while (i < query.length() && query[i] != '&') {
                value += query[i];
                i++;
            }
            this->_queryParams[key] = value;
            i++;
        }
    }
    else {
        this->_endpoint = uri;
        this->_queryParams.clear();
    }
}


/* Destructor */
RequestProcessor::~RequestProcessor() {}

