//
// Created by detjonmataj on 2024-05-26.
//

#ifndef HTTPSERVER_C_HTTP_METHOD_H
#define HTTPSERVER_C_HTTP_METHOD_H

typedef enum {
    GET,
    POST,
    PUT,
    DELETE,
    HEAD,
    PATCH,
    OPTIONS,
    TRACE,
    CONNECT,
} HTTPMethod;

HTTPMethod serialize_http_method(const char *method);
const char *deserialize_http_method(HTTPMethod method);

#endif //HTTPSERVER_C_HTTP_METHOD_H
