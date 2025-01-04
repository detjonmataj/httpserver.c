//
// Created by detjonmataj on 2024-05-28.
//

#include "router.h"
#include "../http/client/handle_client.h"
#include "../utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

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


HTTPResponse route_request(const ClientContext context) {
    HTTPResponse response = {.protocol_version = "HTTP/1.1", .status_code = 200, .headers = hashmap_init()};
    char *full_path = concat(context.http_server.configs.rootPath, context.request.URI);
    struct stat path_stat;
    char *content;
    long long file_bytes_read;
    uint16_t status_code = 200;

    if (stat(full_path, &path_stat) != 0) {
        full_path = context.http_server.configs.defaultErrorPages[404];
        file_bytes_read = read_file_content(full_path, &content);
        status_code = 404;
    } else {
        if (S_ISDIR(path_stat.st_mode)) {
            bool default_file_exists = false;

            if (context.http_server.configs.defaultFileCount > 0) {
                for (int i = 0; i < context.http_server.configs.defaultFileCount; i++) {
                    char *default_file_full_path;
                    if (full_path[strlen(full_path) - 1] == '/') {
                        default_file_full_path = concat(full_path, context.http_server.configs.defaultFiles[i]);
                    } else {
                        char *temp = concat(full_path, "/");
                        default_file_full_path = concat(temp, context.http_server.configs.defaultFiles[i]);
                        free(temp);
                    }

                    if (access(default_file_full_path, F_OK) == 0) {
                        full_path = default_file_full_path;
                        file_bytes_read = read_file_content(full_path, &content);
                        if (file_bytes_read >= 0) {
                            default_file_exists = true;
                        }
                        break;
                    }
                }
            }

            if (!default_file_exists) {
                if (context.http_server.configs.allowDirectoryListing) {
                    // TODO: Should show directory files if a config allows that.
                    content = "Should show directory files if a config allows that.";
                    file_bytes_read = strlen(content);
                } else {
                    full_path = context.http_server.configs.defaultErrorPages[403];
                    file_bytes_read = read_file_content(full_path, &content);
                    status_code = 403;
                }
            }

        } else if (S_ISREG(path_stat.st_mode)) {
            file_bytes_read = read_file_content(full_path, &content);
        } else {
            printf("%s is neither a regular file nor a directory.\n", full_path);
            full_path = context.http_server.configs.defaultErrorPages[404];
            file_bytes_read = read_file_content(full_path, &content);
            status_code = 404;
        }
    }

    if (file_bytes_read < 0) {
        full_path = context.http_server.configs.defaultErrorPages[500];
        file_bytes_read = read_file_content(full_path, &content);
        if (file_bytes_read < 0) {
            file_bytes_read = strlen(HTML_500_ERROR);
            content = HTML_500_ERROR;
        }
        status_code = 500;
    }

    response.body = content;
    response.status_code = status_code;
    hashmap_insert(response.headers, "Content-Length", lltoa(file_bytes_read));
    hashmap_insert(response.headers, "Connection", "close");
    hashmap_insert(response.headers, "Server", "DMServer");
    return response;
}