#include "DataBase.hpp"

UserDatabase::UserDatabase() {}

UserDatabase::~UserDatabase() {}

void UserDatabase::addUserToDatabase(std::map<std::string, std::string> &user_details) {
    User user;
    
    if (user_details.find("username") == user_details.end()) {
        std::cerr << "Error: Username not provided" << std::endl;
        return;
    } else if (this->userExists(user_details["username"])) {
        std::cerr << "Error: User already exists" << std::endl;
        return;
    }

    std::string username = user_details["username"];
    this->_users[username] = user;

    user.first_name = user_details["first_name"];
    user.last_name = user_details["last_name"];
    // user.age = std::stoi(user_details["age"]);
    user.email = user_details["email"];
}

bool UserDatabase::userExists(const std::string &username) const {
    return this->_users.find(username) != this->_users.end();
}

/* print details of requested user */
void UserDatabase::printUserDetails(const std::string &username) const {
    if (this->userExists(username)) {
        User user = this->_users.at(username);
        std::cout << "First name: " << user.first_name << std::endl;
        std::cout << "Last name: " << user.last_name << std::endl;
        std::cout << "Age: " << user.age << std::endl;
        std::cout << "Email: " << user.email << std::endl;
    } else {
        // this needs to be handled as catching exception
        std::cerr << "Error: User does not exist" << std::endl;
    }
}

/* print all usernames available, without user details */
void UserDatabase::printUsers() const {
    std::map<std::string, User>::const_iterator it = this->_users.begin();
    for (; it != this->_users.end(); ++it) {
        std::cout << "Username: " << it->first << std::endl;
    }
}

/* print users in plain html text that will be returned to the server */
// create a string with html tags and return it
std::string UserDatabase::printUserDetails_html(const std::string &username) const {
    std::ostringstream html;
    html << "<html><head><title>User Details</title></head><body><h1>User Details:</h1><ul>" ;
    if (this->userExists(username)) {
        User user = this->_users.at(username);
        html << "<li>First name: " << user.first_name << "</li>";
        html << "<li>Last name: " << user.last_name << "</li>";
        html << "<li>Age: " << user.age << "</li>";
        html << "<li>Email: " << user.email << "</li>";
    } else {
        html << "<li>Error: User " << username << " does not exist</li>";
    }
    return html.str();
}

/* print all usernames available, without user details */
std::string UserDatabase::printUsers_html() const {
    std::ostringstream html;
    html << "<html><head><title>Users</title></head><body><h1>Users:</h1><ul>";
    std::map<std::string, User>::const_iterator it = this->_users.begin();
    for (; it != this->_users.end(); ++it) {
        html << "<li>Username: " << it->first << "</li>";
    }
    html << "</ul></body></html>";
    return html.str();
}
