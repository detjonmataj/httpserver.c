//
// Created by detjonmataj on 2024-05-14.
//

#include "logger.h"
#include <stdarg.h>
#include <stdio.h>
#include <time.h>

const char *serialize_log_level(LogLevel log_level);

void Log(LogLevel log_level, const char *format, ...) {
    FILE *log_file = fopen("logs/log.txt", "a");
    if (log_file == NULL) {
        perror("Log File");
        return;
    }

    time_t raw_time;
    struct tm *info;
    char formatted_time[64];
    time(&raw_time);
    info = localtime(&raw_time);
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

const char *serialize_log_level(LogLevel log_level) {
    static const char *log_level_names[] = {"TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"};
    return ((log_level <= LOG_FATAL) != 0) ? log_level_names[log_level] : NULL;
}
