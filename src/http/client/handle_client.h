//
// Created by detjonmataj on 2024-06-02.
//

#ifndef HTTPSERVER_C_HANDLE_CLIENT_H
#define HTTPSERVER_C_HANDLE_CLIENT_H

#include "../http_server.h"
#include "../request/http_request.h"

typedef struct {
    HTTPServer http_server;
    HTTPRequest request;
    int client_id;
} ClientContext;

void *handle_client(void *arg);

#endif //HTTPSERVER_C_HANDLE_CLIENT_H
