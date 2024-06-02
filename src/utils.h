//
// Created by detjonmataj on 2024-05-18.
//

#ifndef HTTPSERVER_C_UTILS_H
#define HTTPSERVER_C_UTILS_H

#include <stddef.h>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

char *cstringdup(const char *str);
char *cstringndup(const char *s, size_t n);
size_t cstringnlen(const char *str, size_t max_length);
long long read_file_content(const char *filename, char **content_buffer);
const char *ltoa(long number);
char* concat(const char *s1, const char *s2);

#endif //HTTPSERVER_C_UTILS_H
