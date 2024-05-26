//
// Created by detjon on 2024-05-26.
//

#ifndef HTTPSERVER_C_HTTP_METHOD_H
#define HTTPSERVER_C_HTTP_METHOD_H

#include "../../hashmap/hashmap.h"

typedef enum {
    GET,
    POST,
    PUT,
    DELETE,
    HEAD,
    PATCH,
    OPTIONS,
    TRACE,
    CONNECT
} HTTPMethod;

HTTPMethod serialize_http_method(char *method);
const char *deserialize_http_method(HTTPMethod method);

#endif //HTTPSERVER_C_HTTP_METHOD_H
