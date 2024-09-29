#include "../incl/Cgi.hpp"

Environment::Environment(int n) : size(n) {
    env = new char*[size];
    for (int i = 0; i < size; ++i) {
        env[i] = NULL;
    }
}

Environment::~Environment()
{
    for (int i = 0; i < size; ++i) {
        if (env[i] != NULL)
            delete[] env[i];
    } 
    delete[] env;
}

void Environment::AddEnvVar(int i, const char *key, const char *value) {
    if (i >= 0 && i < size) {
        int len = std::strlen(key) + std::strlen(value) + 2; // For "name" + "=" + "value" + '\0'
        env[i] = new char[len];
        std::strcpy(env[i], key);
        std::strcat(env[i], "=");
        std::strcat(env[i], value);
    }
}

void Environment::printEnv() const {
    for (int i = 0; i < size; ++i) {
        std::cout << env[i] << std::endl;
    }
}



// /* CGI class implementation */
// // constructor
// CGI::CGI(void) : env(3) {
//     _python = "/usr/bin/python3";
//     _path_to_script = "./var/www/cgi-bin/hello.py";
//     env.AddEnvVar(0, "REQUEST_METHOD", "GET");
//     env.AddEnvVar(1, "SCRIPT_FILENAME", _path_to_script.c_str());

//     _argv[0] = const_cast<char*>(_python.c_str());
//     _argv[1] = const_cast<char*>(_path_to_script.c_str());
//     _argv[2] = NULL;
// }

// void CGI::executeCGI(void) {

//     // pipe for communication between parent and child
//     int pipefd[2];
//     if (pipe(pipefd) == -1) {
//         throw std::runtime_error("pipe");
//     }


//     pid_t pid = fork();
//     if (pid < 0) {
//         throw std::runtime_error("fork");
//     }
//     else if (pid == 0)  // child
//     {
//         close(pipefd[0]);
//         dup2(pipefd[1], STDOUT_FILENO);
//         close(pipefd[1]);
//         execve(_python.c_str(), _argv, env.env);
//         throw std::runtime_error("execve");
//     }
//     else // parent
//     {
//         close(pipefd[1]);
//         char buf[10000];
//         int nbytes = read(pipefd[0], buf, sizeof(buf)); // don't forget to do it in a loop later
//         if (nbytes == -1) {
//             throw std::runtime_error("read");
//         }

//         int status;
//         waitpid(pid, &status, 0);
//     }
// }