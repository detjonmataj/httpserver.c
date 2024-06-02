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
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

static char *HTML_500_ERROR =
    "<!DOCTYPE html>\n"
    "<html lang=\"en\">\n"
    "<head>\n"
    "    <meta charset=\"UTF-8\">\n"
    "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
    "    <title>500 Internal Server Error</title>\n"
    "</head>\n"
    "<body>\n"
    "    <h1>500 Internal Server Error</h1>\n"
    "</body>\n"
    "</html>";

void *handle_client(void *arg) {
    const ClientContext context = *((ClientContext *)arg);
    const int client_socket = context.client_id;
    char response_buff[4096];

    const long bytes_read = recv(client_socket, response_buff, sizeof(response_buff), 0);
    if (bytes_read < 0) {
        Log(LOG_ERROR, "Error receiving data from client");
        close(client_socket);
        pthread_exit(NULL);
    }

    // Ensure the string is null terminated
    response_buff[bytes_read] = '\0';
    Log(LOG_DEBUG, "Received request: %s", response_buff);

    const HTTPRequest request = parse_http_request(response_buff);

    HTTPResponse response;
    response.protocol_version = "HTTP/1.1";
    response.status_code = 200;
    response.headers = hashmap_init();

    hashmap_insert(response.headers, "Content-Type", "text/html");
    hashmap_insert(response.headers, "Connection", "close");
    hashmap_insert(response.headers, "Server", "DMServer");

    char *path = concat(context.http_server.configs.rootPath, request.URI);
    struct stat path_stat;
    char *content;
    long long file_bytes_read;

    if (stat(path, &path_stat) != 0) {
        path = context.http_server.configs.defaultErrorPages[404];
        file_bytes_read = read_file_content(path, &content);
        if(file_bytes_read < 0) {
            path = context.http_server.configs.defaultErrorPages[500];
            file_bytes_read = read_file_content(path, &content);
            if(file_bytes_read < 0) {
                file_bytes_read = sizeof(HTML_500_ERROR);
                content = HTML_500_ERROR;
            }
        }
    } else {
        if (S_ISDIR(path_stat.st_mode)) {
            content = "Should show directory files if a config allows that.";
            file_bytes_read = strlen(content);
        } else if (S_ISREG(path_stat.st_mode)) {
            file_bytes_read = read_file_content(path, &content);
        } else {
            printf("%s is neither a regular file nor a directory.\n", path);
            exit(EXIT_FAILURE);
        }
    }


    hashmap_insert(response.headers, "Content-Length", ltoa(file_bytes_read));
    response.body = content;

    const char *serialized_http_message = serialize_http_response(response);

    if (send(client_socket, serialized_http_message, strlen(serialized_http_message), 0)) Log(LOG_ERROR, "Failed sending the response");
    close(client_socket);
    Log(LOG_DEBUG, "Response sent to client");
    return NULL;
}
