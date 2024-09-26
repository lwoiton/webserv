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
        std::strcat(env[i], "value");
    }
}

void Environment::printEnv() const {
    for (int i = 0; i < size; ++i) {
        std::cout << env[i] << std::endl;
    }
}