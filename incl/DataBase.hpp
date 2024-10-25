#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <iostream>
#include <map>
#include <string>
#include <sstream>

struct User {
    std::string first_name;
    std::string last_name;
    int age;
    std::string email;
};

class UserDatabase {
    private:
        std::map<std::string, User> _users;
    public:
        UserDatabase();
        ~UserDatabase();

        // Database operations
        void addUserToDatabase(const std::string &username, const User &user_details);
        bool userExists(const std::string &username) const;
        void printUserDetails(const std::string &username) const;
        std::string printUserDetails_html(const std::string &username) const;
        void printUsers() const;
        void printUsers_html() const;
};

#endif