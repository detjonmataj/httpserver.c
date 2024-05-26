//
// Created by detjonmataj on 2024-05-18.
//

#ifndef HTTPSERVER_C_UTILS_H
#define HTTPSERVER_C_UTILS_H

#include <stddef.h>

char *cstringdup(const char *str);
char *cstringndup(const char *s, size_t n);
size_t cstringnlen(const char *str, size_t max_length);
long read_file_content(const char *filename, char **content);
const char *ltoa(long number);

#endif //HTTPSERVER_C_UTILS_H
