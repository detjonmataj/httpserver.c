//
// Created by detjonmataj on 2024-05-13.
//

#ifndef HTTPSERVER_C_HTTP_REQUEST_H
#define HTTPSERVER_C_HTTP_REQUEST_H

#include "../../hashmap/hashmap.h"
#include "http_method.h"

typedef struct {
    HTTPMethod method;
    char *URI;
    char *protocol_version;
    Hashmap *headers;
    char *body;
} HTTPRequest;

HTTPRequest parse_http_request(char *request);

#endif //HTTPSERVER_C_HTTP_REQUEST_H
