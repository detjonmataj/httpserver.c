//
// Created by detjonmataj on 2024-05-11.
//

#ifndef HTTPSERVER_C_SERVER_H
#define HTTPSERVER_C_SERVER_H

#include <netinet/in.h>
#include <sys/socket.h>

typedef struct {
    int domain;
    int service;
    int protocol;
    uint32_t interface;
    int port;
    int backlog;
    struct sockaddr_in address;
    int socket;
} Server;

Server server_init(int domain, int service, int protocol, uint32_t interface, int port, int backlog);

#endif //HTTPSERVER_C_SERVER_H
