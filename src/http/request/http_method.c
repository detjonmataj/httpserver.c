//
// Created by detjonmataj on 2024-05-26.
//

#include "http_method.h"
#include "../../utils.h"
#include "string.h"

static const char *method_names[] = {
    [GET] = "GET",
    [POST] = "POST",
    [PUT] = "PUT",
    [DELETE] = "DELETE",
    [HEAD] = "HEAD",
    [PATCH] = "PATCH",
    [OPTIONS] = "OPTIONS",
    [TRACE] = "TRACE",
    [CONNECT] = "CONNECT",
};

HTTPMethod serialize_http_method(const char *method) {
    const int num_methods = ARRAY_SIZE(method_names);
    for (int i = 0; i < num_methods; ++i) {
        if (strcmp(method, method_names[i]) == 0) {
            return i;
        }
    }
    return -1; // TODO: USE sentinel values
}

const char *deserialize_http_method(HTTPMethod method) {
    return ((method <= CONNECT) != 0) ? method_names[method] : NULL;
}
