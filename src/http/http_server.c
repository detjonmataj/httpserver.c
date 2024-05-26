//
// Created by detjonmataj on 2024-05-11.
//

#include "http_server.h"
#include "../logger/logger.h"
#include "../utils.h"
#include "request/http_request.h"
#include "response/http_response.h"
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

void start(struct HTTPServer *server);
void *handle_client(void *arg);

HTTPServer http_server_init(void) {
    HTTPServer server;
    server.server = server_init(AF_INET, SOCK_STREAM, 0, INADDR_ANY, 27015, 255);
    server.start = start;
    return server;
}

void start(HTTPServer *server) {
    Log(LOG_INFO, "Server started listening on port %d", server->server.port);
    while (1) {
        struct sockaddr *sock_addr = (struct sockaddr *)&server->server.address;
        socklen_t address_length = sizeof(server->server.address);
        int client = accept(server->server.socket, sock_addr, &address_length);

        if (client < 0) {
            Log(LOG_ERROR, "Error accepting connection on port %d", server->server.port);
            exit(EXIT_FAILURE);
        }

        pthread_t tid;
        if (pthread_create(&tid, NULL, handle_client, &client) != 0) {
            Log(LOG_ERROR, "Error creating thread for client connection");
            close(client);
        } else {
            pthread_detach(tid);
            Log(LOG_DEBUG, "Thread created for handling connection from client %d", client);
        }
    }
    Log(LOG_INFO, "Stopping the server.");
}

void *handle_client(void *arg) {
    int client_socket = *((int *)arg);
    char response_buff[4096];

    long bytes_read = recv(client_socket, response_buff, sizeof(response_buff), 0);
    if (bytes_read < 0) {
        Log(LOG_ERROR, "Error receiving data from client");
        close(client_socket);
        pthread_exit(NULL);
    }

    // Ensure the string is null terminated
    response_buff[bytes_read] = '\0';
    Log(LOG_DEBUG, "Received request: %s", response_buff);

    HTTPRequest request = parse_http_request(response_buff);
    (void)request;

    // TODO: Add file-base routing and then add hooks for for integrating with programing languages modules
    // TODO: These values should be dynamic based on the configurations
    HTTPResponse response;
    response.protocol_version = "HTTP/1.1";
    response.status_code = 200;
    response.headers = hashmap_init();

    hashmap_insert(response.headers, "Content-Type", "text/html");
    hashmap_insert(response.headers, "Connection", "close");
    hashmap_insert(response.headers, "Server", "DMServer");
    char *content;
    long file_bytes_read = read_file_content("./wwwroot/index.html", &content);
    hashmap_insert(response.headers, "Content-Length", ltoa(file_bytes_read));
    response.body = content;

    const char *serialized_http_message = serialize_http_response(response);

    send(client_socket, serialized_http_message, strlen(serialized_http_message), 0);
    close(client_socket);
    Log(LOG_DEBUG, "Response sent to client");
    return NULL;
}
