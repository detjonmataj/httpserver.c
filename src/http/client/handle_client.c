//
// Created by detjonmataj on 2024-06-02.
//

#include "handle_client.h"
#include <sys/stat.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include "../../utils.h"
#include "../request/http_request.h"
#include "../response/http_response.h"
#include "../../router/router.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void *handle_client(void *arg) {
    ClientContext context = *((ClientContext *)arg);
    const int client_socket = context.client_id;
    char request_buff[4096];

    const long bytes_read = recv(client_socket, request_buff, sizeof(request_buff), 0);
    if (bytes_read < 0) {
        Log(LOG_ERROR, "Error receiving data from client");
        close(client_socket);
        pthread_exit(NULL);
    }

    // Ensure the string is null terminated
    request_buff[bytes_read] = '\0';
    Log(LOG_DEBUG, "Received request: %s", request_buff);

    context.request = parse_http_request(request_buff);

    HTTPResponse response = route_request(context);

    const char *serialized_http_message = serialize_http_response(response);

    if (send(client_socket, serialized_http_message, strlen(serialized_http_message), 0)) Log(LOG_ERROR, "Failed sending the response");
    close(client_socket);
    Log(LOG_DEBUG, "Response sent to client");
    return NULL;
}
