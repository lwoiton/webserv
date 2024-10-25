#include "RequestProcessor.hpp"

/* Constructor */
RequestProcessor::RequestProcessor(Request *req, Response *res, UserDatabase *userDB) : _parsedRequest(req), _response(res), _usersDB(userDB) {
    this->_request = req->getMethod();
    parseURI(req->getUri());
}

void RequestProcessor::processRequest() {
    if (this->_request == "GET") {
        this->handleGETRequest();
    } else if (this->_request == "POST") {
        this->handlePOSTRequest();
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

    if (this->_endpoint == "/" || this->_endpoint == "/ravicon.ico") {
        _response->setStatus(200, "OK");
        _response->addHeader("Content-Type", "text/html");
        _response->setBody(readFile("./public/index.html"));
        _response->addHeader("Content-Length", _sizeToString(_response->getBody().length()));
    } else if (this->_endpoint.find(".html") != std::string::npos) {
        _response->setStatus(200, "OK"); // this is not correct fully, it should be based on the file
        _response->addHeader("Content-Type", "text/html");
        _response->setBody(readFile("./public" + this->_endpoint));
        _response->addHeader("Content-Length", _sizeToString(_response->getBody().length()));
    } else if (this->_endpoint == "/users") {
        std::string res = _usersDB->printUsers_html();
        _response->setStatus(200, "OK");
        _response->addHeader("Content-Type", "text/html");
        _response->setBody(res);
        _response->addHeader("Content-Length", _sizeToString(_response->getBody().length()));
    } 
    else if (this->_endpoint == "/users/get_user") {
        std::string res = _usersDB->printUserDetails_html(this->_queryParams["username"]);
        _response->setStatus(200, "OK");
        _response->addHeader("Content-Type", "text/html");
        _response->setBody(res);
        _response->addHeader("Content-Length", _sizeToString(_response->getBody().length()));
    } else {
        _response->setStatus(404, "Not Found");
        _response->addHeader("Content-Type", "text/html");
        _response->setBody("<html><body><h1>404 Not Found</h1></body></html>");
        _response->addHeader("Content-Length", _sizeToString(_response->getBody().length()));
    }
}

void RequestProcessor::handlePOSTRequest() {
    std::string url = _parsedRequest->getUri();
    parseURI(_parsedRequest->getBody());
    std::cout << "POST request for: " << url << std::endl;
    std::cout << "{" << url << "}" << std::endl;
    std::cout << "{" << this->_endpoint << "}" << std::endl;
    std::cout << "{" << _parsedRequest->getBody() << "}" << std::endl;

    if (this->_endpoint == "/user_create") {
        _usersDB->addUserToDatabase(this->_queryParams);
    }
}

/* Parse URL encoded data */
void RequestProcessor::parseURI(std::string _uri) {
    std::string uri = _uri;
    decodeURI(uri);
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

/* Decode URL encoded data */
void RequestProcessor::decodeURI(std::string &uri) {
    std::string decoded;
    size_t pos = 0;
    while (pos < uri.length()) {
        if (uri[pos] == '%') {
            std::string hex = uri.substr(pos + 1, 2);
            int x;
            sscanf(hex.c_str(), "%x", &x);
            decoded += x;
            pos += 3;
        } else {
            decoded += uri[pos];
            pos++;
        }
    }
    uri = decoded;
}

/* Destructor */
RequestProcessor::~RequestProcessor() {}

