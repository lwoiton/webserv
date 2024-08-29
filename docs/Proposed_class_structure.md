class ConfigParser {
private:
    std::map<std::string, std::string> config;
public:
    ConfigParser(const std::string& filename);
    std::string getValue(const std::string& key) const;
    // Other methods to parse and access configuration
};

class HttpRequest {
private:
    std::string method;
    std::string uri;
    std::string httpVersion;
    std::map<std::string, std::string> headers;
    std::string body;
public:
    HttpRequest();
    void parse(const std::string& rawRequest);
    // Getters for various components
};

class HttpResponse {
private:
    int statusCode;
    std::string statusMessage;
    std::map<std::string, std::string> headers;
    std::string body;
public:
    HttpResponse();
    void setStatus(int code, const std::string& message);
    void addHeader(const std::string& key, const std::string& value);
    void setBody(const std::string& content);
    std::string toString() const;
};

class Router {
private:
    std::map<std::string, std::function<void(const HttpRequest&, HttpResponse&)>> routes;
public:
    void addRoute(const std::string& path, std::function<void(const HttpRequest&, HttpResponse&)> handler);
    void route(const HttpRequest& request, HttpResponse& response);
};

class CGIHandler {
public:
    static void execute(const HttpRequest& request, HttpResponse& response, const std::string& scriptPath);
};

class FileHandler {
public:
    static void serveFile(const HttpRequest& request, HttpResponse& response, const std::string& filePath);
};

class EpollWrapper {
private:
    int epollFd;
    std::vector<struct epoll_event> events;
public:
    EpollWrapper(int maxEvents);
    ~EpollWrapper();
    void addFd(int fd, uint32_t events);
    void modifyFd(int fd, uint32_t events);
    void removeFd(int fd);
    int wait(int timeout);
    epoll_event* getEvent(int index);
};

class Connection {
private:
    int socketFd;
    HttpRequest request;
    HttpResponse response;
public:
    Connection(int fd);
    void readRequest();
    void writeResponse();
    int getSocketFd() const;
};

class Server {
private:
    int serverSocket;
    EpollWrapper epoll;
    Router router;
    std::map<int, std::shared_ptr<Connection>> connections;
public:
    Server(const ConfigParser& config);
    void run();
    void handleNewConnection();
    void handleExistingConnection(int fd);
};