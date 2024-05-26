//
// Created by detjonmataj on 2024-05-13.
//

#ifndef HTTPSERVER_C_HTTP_RESPONSE_H
#define HTTPSERVER_C_HTTP_RESPONSE_H

#include "../../hashmap/hashmap.h"
#include <stdint.h>

typedef struct {
    uint16_t status_code;
    char *protocol_version;
    Hashmap *headers;
    char *body;
} HTTPResponse;

HTTPResponse create_http_response(int status_code, Hashmap *headers, char *body);
void free_http_response(HTTPResponse *response);
const char *serialize_http_response(HTTPResponse response);

#endif //HTTPSERVER_C_HTTP_RESPONSE_H
