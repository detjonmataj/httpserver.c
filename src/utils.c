//
// Created by detjonmataj on 2024-05-18.
//

#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *cstringdup(const char *str) {
    const size_t len = strlen(str) + 1;
    char *dup = malloc(len);
    if (dup) {
        memcpy(dup, str, len);
    }
    return dup;
}

char *cstringndup(const char *s, size_t n) {
    const size_t len = cstringnlen(s, n);
    char *dup = malloc(len + 1);

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

long long read_file_content(const char *filename, char **content_buffer) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Unable to open file '%s'\n", filename);
        return -1;
    }

    fseek(file, 0, SEEK_END);
    const long file_size = ftell(file);
    rewind(file);

    *content_buffer = (char *)malloc(file_size + 1);
    if (*content_buffer == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        fclose(file);
        return -1;
    }

    const long bytes_read = fread(*content_buffer, 1, file_size, file);
    if (bytes_read != file_size) {
        fprintf(stderr, "Error: Unable to read file '%s'\n", filename);
        fclose(file);
        free(*content_buffer);
        return -1;
    }

    (*content_buffer)[file_size] = '\0';

    fclose(file);

    return bytes_read;
}

const char *ltoa(const long number) {
    static char str[12];
    snprintf(str, sizeof(str), "%ld", number);
    return str;
}

const char *lltoa(long number) {
    static char str[21];
    snprintf(str, sizeof(str), "%ld", number);
    return str;
}

char* concat(const char *s1, const char *s2) {
    const size_t len1 = strlen(s1);
    const size_t len2 = strlen(s2);
    
    char *result = malloc(len1 + len2 + 1);
    if (result == NULL) {
        fprintf(stderr, "You need more ram\n");
        exit(EXIT_FAILURE);
    }
    
    strcpy(result, s1);
    strcat(result, s2);
    
    return result;
}