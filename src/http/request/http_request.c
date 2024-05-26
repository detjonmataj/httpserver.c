//
// Created by detjonmataj on 2024-05-13.
//

#include "http_request.h"
#include "../../logger/logger.h"
#include "../../utils.h"
#include "http_method.h"
#include <assert.h>
#include <string.h>

HTTPRequest parse_http_request(char *request) {
    HTTPRequest http_request = {
        .headers = hashmap_init(),
    };
    size_t request_length = strlen(request);
    size_t pos = 0;
    size_t token_start = 0;
    size_t token_length = 0;
    // 0 - Parsing HTTP method/verb
    // 1 - Parsing URI
    // 2 - Parsing HTTP protocol version
    // 3 - Parsing Request Headers
    // 4 - Parsing Request Body
    int state = 0;

    while (pos < request_length) {
        switch (state) {
            // Parsing method, URI, and protocol version (Request Line)
            case 0:
            case 1:
            case 2:
                while (pos < request_length && request[pos] != ' ' && request[pos] != '\r' && request[pos] != '\n') {
                    token_length++;
                    pos++;
                }

                if (state == 0) {
                    http_request.method = serialize_http_method(cstringndup(request + token_start, token_length));
                } else if (state == 1) {
                    http_request.URI = cstringndup(request + token_start, token_length);
                } else /* state == 2 */ {
                    http_request.protocol_version = cstringndup(request + token_start, token_length);
                }

                token_length = 0;
                state++;
                if (request[pos] == '\r' && pos + 1 < request_length && request[pos + 1] == '\n') {
                    pos++; // Skip the '\n'
                }
                token_start = pos + 1;
                break;

            // Parsing headers
            case 3:
                if (request[pos] == '\r' && pos + 1 < request_length && request[pos + 1] == '\n') {
                    char *colon_position = strchr(request + token_start, ':');
                    // TODO: If there is no colon it means that the HTTP header is invalid. Investigate what should be done in that case.
                    // TODO See if parsing like
                    if (colon_position != NULL) {
                        *colon_position = '\0';
                        // it should be \r (CR), however i'm using temp just in case
                        char temp = request[token_start + token_length];
                        request[token_start + token_length] = '\0';
                        char *key = request + token_start;
                        char *value = colon_position + 1;
                        // Remove trailing spaces
                        while (*value == ' ') value++;
                        hashmap_insert(http_request.headers, key, value);
                        *colon_position = ':';
                        request[token_start + token_length] = temp;
                    } else {
                        Log(LOG_WARNING, "Invalid HTTP header format: %s", request + token_start);
                    }

                    if (pos + 2 < request_length && request[pos + 2] == '\r' && pos + 3 < request_length && request[pos + 3] == '\n') {
                        state++; // Transition to parsing the body
                        token_start = pos + 4;
                        pos += 3; // Skip the next '\r\n\r\n'
                    } else {
                        token_start = pos + 2;
                        token_length = 0;
                        pos++; // Skip the '\n'
                    }
                } else {
                    token_length++;
                }
                break;

            // Parsing body
            case 4:
                token_length = request_length - token_start;
                if (token_length > 0) {
                    http_request.body = cstringndup(request + token_start, token_length);
                } else {
                    http_request.body = NULL;
                }
                pos = request_length; // Exit loop
                break;
            default:
                assert(NULL && "How did this bug get in here?");
        }

        pos++;
    }

    Log(LOG_DEBUG, "HTTP Request parsed successfully");

    return http_request;
}
