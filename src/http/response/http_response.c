//
// Created by detjonmataj on 2024-05-13.
//

#include "http_response.h"
#include "../../utils.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// https://en.cppreference.com/w/c/language/array_initialization
static const char *status_texts[] = {
    // Informational 1xx
    [100] = "Continue",
    [101] = "Switching Protocols",
    // Success 2xx
    [200] = "OK",
    [201] = "Created",
    [202] = "Accepted",
    [203] = "Non-Authoritative Information",
    [204] = "No Content",
    [205] = "Reset Content",
    [206] = "Partial Content",
    // Redirection 3xx
    [300] = "Multiple Choices",
    [301] = "Moved Permanently",
    [302] = "Found",
    [303] = "See Other",
    [304] = "Not Modified",
    [305] = "Use Proxy",
    [307] = "Temporary Redirect",
    // Client Error 4xx
    [400] = "Bad Request",
    [401] = "Unauthorized",
    [402] = "Payment Required",
    [403] = "Forbidden",
    [404] = "Not Found",
    [405] = "Method Not Allowed",
    [406] = "Not Acceptable",
    [407] = "Proxy Authentication Required",
    [408] = "Request Timeout",
    [409] = "Conflict",
    [410] = "Gone",
    [411] = "Length Required",
    [412] = "Precondition Failed",
    [413] = "Payload Too Large",
    [414] = "URI Too Long",
    [415] = "Unsupported Media Type",
    [416] = "Range Not Satisfiable",
    [417] = "Expectation Failed",
    [426] = "Upgrade Required",
    // Server Error 5xx
    [500] = "Internal Server Error",
    [501] = "Not Implemented",
    [502] = "Bad Gateway",
    [503] = "Service Unavailable",
    [504] = "Gateway Timeout",
    [505] = "HTTP Version Not Supported",
};

HTTPResponse create_http_response(const char *protocol_version, const int status_code, Hashmap *headers, char *body) {
    HTTPResponse response;
    response.protocol_version = protocol_version;
    response.status_code = status_code;
    response.headers = headers;
    response.body = body;
    return response;
}

void free_http_response(const HTTPResponse *response) {
    if (response->headers != NULL) {
        hashmap_free(response->headers);
    }
    if (response->body != NULL) {
        free(response->body);
    }
}

const char *serialize_http_response(const HTTPResponse response) {
    const char *status_text = (response.status_code < sizeof(status_texts) / sizeof(status_texts[0])) ? status_texts[response.status_code] : NULL;

    // TODO: The checks are redundant. Need to make sure  that the http response object is always valid.
    // Maybe this will become a problem when using modules for integrating with programming languages
    // In that case maybe it can be defaulted at a higher level (configuration?)
    const size_t protocol_version_length = (response.protocol_version != NULL) ? strlen(response.protocol_version) : 0;
    const size_t status_text_length = (status_text != NULL) ? strlen(status_text) : 0;
    const size_t status_code_length = 3; // HTTP status codes are 3 digits
    const size_t status_line_length = protocol_version_length + 1 + status_code_length + 1 + status_text_length + 2; // 1 space before status code, 1 space before status text, 2 for "\r\n"

    size_t headers_length = 0;
    if (response.headers != NULL) {
        HashmapIterator *iterator = hashmap_iterator_init(response.headers);
        Entry *entry;
        while ((entry = hashmap_iterator_next(iterator)) != NULL) {
            headers_length += strlen(entry->key) + 2 + strlen(entry->value) + 2; // 2 for ": ", 2 for "\r\n"
        }
        hashmap_iterator_free(iterator);
    }

    // TODO: Include the length of the body in the http response struct or pull it from Content-Length Header instead
    const size_t body_length = (response.body != NULL) ? strlen(response.body) : 0;

    const size_t buffer_size = status_line_length + headers_length + body_length + 3; // 3 for "\r\n\r\n" at the end
    char *buffer = malloc((buffer_size + 1) * sizeof(char)); // +1 for null terminator

    if (buffer == NULL) {
        assert(NULL && "You need more RAM bro!");
    }

    if (response.protocol_version != NULL) {
        snprintf(buffer, buffer_size, "%s %d %s\r\n", response.protocol_version, response.status_code, status_text);
    } else {
        snprintf(buffer, buffer_size, "HTTP/1.1 %d %s\r\n", response.status_code, status_text);
    }

    if (response.headers != NULL) {
        HashmapIterator *iterator = hashmap_iterator_init(response.headers);
        Entry *entry;
        while ((entry = hashmap_iterator_next(iterator)) != NULL) {
            const size_t len = strlen(buffer);
            snprintf(buffer + len, buffer_size - len, "%s: %s\r\n", entry->key, entry->value);
        }
        hashmap_iterator_free(iterator);
    }

    // CRLF in between the headers and body
    // https://datatracker.ietf.org/doc/html/rfc2616#section-6
    strncat(buffer, "\r\n", buffer_size - strlen(buffer) - 1);

    if (response.body != NULL) {
        strncat(buffer, response.body, buffer_size - strlen(buffer) - 1);
    }

    return buffer;
}
