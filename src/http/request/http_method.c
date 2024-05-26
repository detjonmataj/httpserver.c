//
// Created by detjon on 2024-05-26.
//

#include "http_method.h"
#include "string.h"

static const char *method_names[] = {"GET", "POST", "PUT", "DELETE", "HEAD", "PATCH", "OPTIONS", "TRACE", "CONNECT"};

HTTPMethod serialize_http_method(char *method) {
    const int num_methods = sizeof(method_names) / sizeof(method_names[0]);

    for (int i = 0; i < num_methods; ++i) {
        if (strcmp(method, method_names[i]) == 0) {
            return (HTTPMethod)i;
        }
    }

    return -1; // TODO: USE sentinel values
}

const char *deserialize_http_method(HTTPMethod method) {
    return ((method <= CONNECT) != 0) ? method_names[method] : NULL;
}
