#ifndef CGI_HPP
# define CGI_HPP

#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

class Environment {

    public: 
        char **env;
        int size;

    Environment(int n);
    ~Environment();

    void AddEnvVar(int index, const char *key, const char *value);
    void printEnv() const;
    // getter
    char **getEnv() const { return env; }
};


// class CGI
// {
//     private:
//         Environment env;
//         std::string _python;
//         std::string _path_to_script;
//         char * _argv[];
//     public:
//         CGI(void);
//         ~CGI(void);
//         void executeCGI(void);
// };

#endif