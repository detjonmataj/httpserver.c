//
// Created by detjonmataj on 2024-05-28.
//

#ifndef HTTPSERVER_C_ROUTER_H
#define HTTPSERVER_C_ROUTER_H

#include "../http/client/handle_client.h"
#include "../http/response/http_response.h"

HTTPResponse route_request(ClientContext context);

#endif //HTTPSERVER_C_ROUTER_H
