//
// Created by detjonmataj on 2024-05-11.
//

#include "http/http_server.h"
#include <unistd.h>

int main(void) {
    const HTTPServer httpServer = http_server_init();
    httpServer.start(&httpServer);
    close(httpServer.server.socket);
    return 0;
}
