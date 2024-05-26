//
// Created by detjonmataj on 2024-05-18.
//

#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *cstringdup(const char *str) {
    size_t len = strlen(str) + 1;
    char *dup = malloc(len);
    if (dup) {
        memcpy(dup, str, len);
    }
    return dup;
}

char *cstringndup(const char *s, size_t n) {
    size_t len = cstringnlen(s, n);
    char *dup = (char *)malloc(len + 1);

    if (dup != NULL) {
        memcpy(dup, s, len);
        dup[len] = '\0';
    }

    return dup;
}

size_t cstringnlen(const char *str, size_t max_length) {
    const char *end = str;
    size_t length = 0;

    while (length < max_length && *end != '\0') {
        end++;
        length++;
    }

    return length;
}

long read_file_content(const char *filename, char **content) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Unable to open file '%s'\n", filename);
        return -1;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    *content = (char *)malloc(file_size + 1);
    if (*content == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        fclose(file);
        return -1;
    }

    long bytes_read = fread(*content, 1, file_size, file);
    if (bytes_read != file_size) {
        fprintf(stderr, "Error: Unable to read file '%s'\n", filename);
        fclose(file);
        free(*content);
        return -1;
    }

    (*content)[file_size] = '\0';

    fclose(file);

    return bytes_read;
}

const char *ltoa(long number) {
    static char str[20];
    snprintf(str, sizeof(str), "%ld", number);
    return str;
}
