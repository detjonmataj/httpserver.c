//
// Created by detjonmataj on 2024-05-11.
//

#include "server.h"
#include <stdio.h>
#include <stdlib.h>

Server server_init(const int domain, const int service, const int protocol, const uint32_t interface, const int port, const int backlog) {
    const Server server = {
        .domain = domain,
        .service = service,
        .protocol = protocol,
        .interface = interface,
        .port = port,
        .backlog = backlog,
        .address = {
            .sin_family = domain,
            .sin_port = htons(port),
            .sin_addr = {
                .s_addr = htonl(interface),
            },
        },
        .socket = socket(domain, service, protocol),
    };

    // Confirm the connection was successful.
    if (server.socket == 0) {
        perror("Cannot connect to socket");
        exit(EXIT_FAILURE);
    }

    // Attempt to bind the socket to the network.
    if ((bind(server.socket, (const struct sockaddr *)&server.address, sizeof(server.address))) < 0) {
        perror("Cannot bind socket");
        exit(EXIT_FAILURE);
    }

    // Start listening on the network.
    if ((listen(server.socket, server.backlog)) < 0) {
        perror("Cannot to start listening");
        exit(EXIT_FAILURE);
    }

    return server;
}
