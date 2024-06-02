//
// Created by detjonmataj on 2024-05-11.
//

#include "http_server.h"
#include "../logger/logger.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include "./client/handle_client.h"

void start(const HTTPServer *server);

HTTPServer http_server_init(void) {
    HTTPServer server;
    server.configs = parseConfig("./config/server.conf");
    server.server = server_init(AF_INET, SOCK_STREAM, 0, INADDR_ANY, server.configs.port, 255);
    server.start = start;
    return server;
}

void start(const HTTPServer *server) {
    Log(LOG_INFO, "Server started listening on port %d", server->server.port);
    while (1) {
        struct sockaddr *sock_addr = (struct sockaddr *)&server->server.address;
        socklen_t address_length = sizeof(server->server.address);
        const int client = accept(server->server.socket, sock_addr, &address_length);

        if (client < 0) {
            Log(LOG_ERROR, "Error accepting connection on port %d", server->server.port);
            exit(EXIT_FAILURE);
        }

        ClientContext *context = malloc(sizeof(ClientContext));
        context->client_id = client;
        context->http_server = *server;

        pthread_t tid;
        if (pthread_create(&tid, NULL, handle_client, context) != 0) {
            Log(LOG_ERROR, "Error creating thread for client connection");
            close(client);
        } else {
            pthread_detach(tid);
            Log(LOG_DEBUG, "Thread created for handling connection from client %d", client);
        }
    }
    Log(LOG_INFO, "Stopping the server.");
}
