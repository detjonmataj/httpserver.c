//
// Created by detjonmataj on 2024-05-31.
//

#ifndef HTTPSERVER_C_CONFIG_H
#define HTTPSERVER_C_CONFIG_H
#include "../logger/logger.h"
#include <stdbool.h>

typedef struct {
    char *rootPath;
    char **defaultFiles;
    int defaultFileCount;
    int port;
    bool enableLogging;
    LogLevel logLevel;
    char **defaultErrorPages;
} HttpConfig;

HttpConfig parseConfig(const char *filename);
void freeConfig(HttpConfig config);

#endif //HTTPSERVER_C_CONFIG_H
