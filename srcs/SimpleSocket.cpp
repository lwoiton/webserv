#include "SimpleSocket.hpp"

/*================================ DEFAULT CONSTRUCTOR ================================*/

SimpleSocket::SimpleSocket(const char *input_node, const char *input_service)
{
    node = input_node;
    service = input_service;

    if (fill_addrinfo(node, service) != 0) {
        std::cerr << "Error: fill_addrinfo()" << std::endl;
        exit(1);
    }

    if (create_socket() != 0) {
        std::cerr << "Error: create_socket()" << std::endl;
        exit(2);
    }

    if (listening_socket() != 0) {
        std::cerr << "Error: listening_socket()" << std::endl;
        exit(3);
    }
}

/*================================ MEMBER FUNCTIONS ================================*/

/* Fill in addrinfo struct using getaddrinfo() function; */
int SimpleSocket::fill_addrinfo(const char *node, const char *service)
{
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int status = getaddrinfo(node, service, &hints, &add_info);
    if (status != 0) {
        std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
        return (1);
    }

    return (0);
}

/*
Iterate through the linked list creating in fill_addrinfo() function, and find a first successful
socket connection. 
If successful connection is found, we break the loop and free *res linked list, return 0.
If no successfull connection found, free *res linked list, return 1.
*/
int SimpleSocket::create_socket()
{
    struct addrinfo *p;
    int i = 0;
    // iterate through the linked list addrinfo *res to find first successfull connection
    for (p = add_info; p != NULL; p = p->ai_next)
    {
        // get socket file descriptor
        i++;
        socket_fd = socket(p->ai_family, p->ai_socktype | SOCK_NONBLOCK, p->ai_protocol);
        if (socket_fd < 0) {
            std::cerr << "Error: Socket() " << i << std::endl;
            continue;
        }
        else
            std::cout << "Socket is created." << i << std::endl;

        // establish connection to a network
        int connection = bind(socket_fd, p->ai_addr, p->ai_addrlen);
        if (connection < 0) {
            std::cerr << "Error: Bind() " << i << std::endl;
            close(socket_fd);
            continue;
        }
        else
            std::cout << "Socket is binded to a network." << i << std::endl;

        // successfull connection is established. 
        break; 
    }

    if (p == NULL) {
        std::cerr << "No successfull connection has been found." << i << std::endl;
        freeaddrinfo(add_info);
        return (1);
    }
    // free linked list as we don't need it anymore after establishing a connection
    freeaddrinfo(add_info);

    return (0);
}
/* Create a listening socket that will be waiting and monitoring the incoming client connections */
int SimpleSocket::listening_socket() {

    int listening = listen(get_socket(), BACKLOG);

    if (listening < 0) {
        std::cout << "Error: Listen()" << std::endl;
        return (1);
    }

    std::cout << "Socket started listening." << std::endl;

    return (0);
}

/*================================ GETTERS ================================*/

const struct addrinfo &SimpleSocket::get_address() const {
    //if (add_info != NULL)
        return *add_info;
}

int SimpleSocket::get_socket() {
    return socket_fd;
}