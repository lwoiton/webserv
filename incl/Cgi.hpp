#ifndef CGI_HPP
# define CGI_HPP

#include <cstring>
#include <iostream>

class Environment {

    public: 
        char **env;
        int size;

    Environment(int n);
    ~Environment();

    void AddEnvVar(int index, const char *key, const char *value);
    void printEnv() const;
};


#endif