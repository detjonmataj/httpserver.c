//
// Created by detjonmataj on 2024-05-31.
//

#include "config.h"
#include "../utils.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *defaultErrorPages[] = {
    [400] = NULL,
    [401] = NULL,
    [403] = NULL,
    [404] = NULL,
    [500] = NULL,
    [501] = NULL,
    [502] = NULL,
    [503] = NULL,
    [504] = NULL,
    NULL, // Sentinel value to mark the end of the array
};

char *trimWhitespaceAndQuotes(char *str) {
    while (*str == ' ' || *str == '\t' || *str == '\n' || *str == '"')
        str++;
    size_t len = strlen(str);
    while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\t' || str[len - 1] == '\n' || str[len - 1] == '"'))
        str[--len] = '\0';
    return str;
}

// TODO: Validate config values
HttpConfig parseConfig(const char *filename) {
    HttpConfig config = {0};
    FILE *file = fopen(filename, "r");

    if (!file) {
        fprintf(stderr, "Error opening file %s\n", filename);
        exit(1);
    }

    config.defaultErrorPages = defaultErrorPages;
    char line[512];
    while (fgets(line, sizeof(line), file)) {
        char *key = strtok(line, "=");
        char *value = strtok(NULL, "=");

        if (key && value) {
            key = trimWhitespaceAndQuotes(key);
            value = trimWhitespaceAndQuotes(value);

            if (strcmp(key, "RootPath") == 0) {
                config.rootPath = cstringdup(value);
            } else if (strcmp(key, "IndexFiles") == 0) {
                // Parse IndexFiles array
                char *token = strtok(value, "[],");
                while (token != NULL) {
                    // Ignore empty tokens
                    if (strlen(token) > 0) {
                        config.defaultFiles = realloc(config.defaultFiles, (config.defaultFileCount + 1) * sizeof(char *));
                        config.defaultFiles[config.defaultFileCount++] = cstringdup(trimWhitespaceAndQuotes(token));
                    }
                    token = strtok(NULL, "[],");
                }
            } else if (strcmp(key, "Port") == 0) {
                config.port = atoi(value);
            } else if (strcmp(key, "EnableLogging") == 0) {
                // TODO: extract this logic to utils
                // Also we need to validate the config eary on so the user doesn't get confused in case of a misconfiguration.
                config.enableLogging = strcmp(value, "true") == 0 ? true : false;
            } else if (strcmp(key, "LogLevel") == 0) {
                config.logLevel = deserialize_log_level(value);
            } else if (strcmp(key, "ErrorPages") == 0) {
                // TODO: Make this logic reusable in case we have other config with key-value pairs
                while (fgets(line, sizeof(line), file) && strstr(line, "}") == NULL) {
                    char *errorCodeToken = strtok(line, "=");
                    char *errorPageToken = strtok(NULL, "=");
                    if (errorCodeToken && errorPageToken) {
                        char *comma = strchr(errorPageToken, ',');
                        if (comma) *comma = '\0'; // Remove coma
                        const int errorCode = atoi(trimWhitespaceAndQuotes(errorCodeToken));
                        char *errorPage = cstringdup(trimWhitespaceAndQuotes(errorPageToken));
                        config.defaultErrorPages[errorCode] = errorPage;
                    }
                }
            } else if (strcmp(key, "AllowDirectoryListing") == 0) {
                config.allowDirectoryListing = strcmp(value, "true") == 0 ? true : false;
            } else {
                // I believe it's better to crash/warn rather than ignoring invalid configs. This will probly help avoiding confusions.
                fprintf(stderr, "Invalid config: '%s'.\n", key);
                exit(EXIT_FAILURE);
            }
        }
    }

    fclose(file);
    return config;
}

void freeConfig(const HttpConfig config) {
    free(config.rootPath);

    for (int i = 0; i < config.defaultFileCount; i++) {
        free(config.defaultFiles[i]);
    }

    free(config.defaultFiles);

    // Free defaultErrorPages array until the sentinel value is encountered
    for (int i = 0; defaultErrorPages[i] != NULL; i++) {
        free(config.defaultErrorPages[i]);
    }
}