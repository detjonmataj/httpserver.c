//
// Created by detjonmataj on 2024-05-14.
//

#include "logger.h"

#include "../utils.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

static const char *log_level_names[] = {
    [LOG_TRACE] = "TRACE",
    [LOG_DEBUG] = "DEBUG",
    [LOG_INFO] = "INFO",
    [LOG_WARNING] = "WARNING",
    [LOG_ERROR] = "ERROR",
    [LOG_FATAL] = "FATAL"};

void Log(const LogLevel log_level, const char *format, ...) {
    FILE *log_file = fopen("logs/log.txt", "a");
    if (log_file == NULL) {
        perror("Log File");
        return;
    }

    time_t raw_time;
    char formatted_time[64];
    time(&raw_time);
    const struct tm *info = localtime(&raw_time);
    strftime(formatted_time, sizeof(formatted_time), "%Y-%m-%d %H:%M:%S", info);

    // Write log level, timestamp, and message to the file
    fprintf(log_file, "[%s] - [%s]: ", formatted_time, serialize_log_level(log_level));

    va_list args;
    va_start(args, format);
    vfprintf(log_file, format, args);
    va_end(args);

    fprintf(log_file, "\n");

    fclose(log_file);
}

const char *serialize_log_level(const LogLevel log_level) {
    return ((log_level <= LOG_FATAL) != 0) ? log_level_names[log_level] : NULL;
}

LogLevel deserialize_log_level(const char *log_level) {
    const int num_levels = ARRAY_SIZE(log_level_names);
    for (int i = 0; i < num_levels; ++i) {
        if (log_level_names[i] != NULL && strcmp(log_level, log_level_names[i]) == 0) {
            return i;
        }
    }
    return -1;
}