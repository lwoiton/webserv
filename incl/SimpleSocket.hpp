#ifndef SIMPLESOCKET_HPP
#define SIMPLESOCKET_HPP

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <iostream>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <cstdlib>

#define BACKLOG 10

class SimpleSocket
{
    private:
        struct addrinfo hints, *add_info;
        int socket_fd;
        const char *node;    // e.g. "www.example.com" or IP
        const char *service; // e.g. "http" or port number

        int fill_addrinfo(const char *node, const char *service);
        int create_socket();
        int listening_socket();

    public:
        SimpleSocket(const char *input_node, const char *input_service);
        //getters
        const struct addrinfo &get_address() const;
        int get_socket();
};

#endif