//
// Created by detjonmataj on 2024-05-11.
//

#ifndef HTTPSERVER_C_HTTP_SERVER_H
#define HTTPSERVER_C_HTTP_SERVER_H

#include "../server/server.h"

typedef struct HTTPServer HTTPServer;
struct HTTPServer {
    Server server;
    void (*start)(HTTPServer *server);
};

HTTPServer http_server_init(void);

#endif //HTTPSERVER_C_HTTP_SERVER_H
