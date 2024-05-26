//
// Created by detjonmataj on 2024-05-14.
//

#ifndef HTTPSERVER_C_LOGGER_H
#define HTTPSERVER_C_LOGGER_H

typedef enum {
    LOG_TRACE,
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_FATAL
} LogLevel;

void Log(LogLevel log_level, const char *format, ...);

#endif //HTTPSERVER_C_LOGGER_H
