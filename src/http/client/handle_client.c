//
// Created by detjonmataj on 2024-06-02.
//

#include "handle_client.h"
#include "../../router/router.h"
#include "../request/http_request.h"
#include "../response/http_response.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

// Important: https://blog.netherlabs.nl/articles/2009/01/18/the-ultimate-so_linger-page-or-why-is-my-tcp-not-reliable
void *handle_client(void *arg) {
    ClientContext context = *(ClientContext *)arg;
    const int client_socket = context.client_id;
    char request_buff[4096];

    // TODO: Reads only 4KB of data, instead it should drain all incoming data n amount of bytes out of time.
    // Maybe we should introduce a config for max req size.
    const long bytes_read = recv(client_socket, request_buff, sizeof(request_buff), 0);
    if (bytes_read < 0) {
        Log(LOG_ERROR, "Error receiving data from client");

        const char *error_response = "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 0\r\n\r\n";
        if (send(client_socket, error_response, strlen(error_response), 0) < 0) {
            Log(LOG_ERROR, "Failed to notify client about error on socket %d", client_socket);
        }

        shutdown(client_socket, SHUT_WR);
        close(client_socket);
        pthread_exit(NULL);
    }

    // Ensure the string is null terminated
    request_buff[bytes_read] = '\0';
    Log(LOG_DEBUG, "Received request: %s", request_buff);

    context.request = parse_http_request(request_buff);

    const HTTPResponse response = route_request(context);

    const char *serialized_http_message = serialize_http_response(response);

    if (send(client_socket, serialized_http_message, strlen(serialized_http_message), 0)) {
        Log(LOG_ERROR, "Failed sending the response");
    }
    shutdown(client_socket, SHUT_WR);
    close(client_socket);
    Log(LOG_DEBUG, "Response sent to client");
    pthread_exit(NULL);
}
