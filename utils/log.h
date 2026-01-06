#ifndef __MIMIR_LOG_H__
#define __MIMIR_LOG_H__

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

#define STDOUT_LOGGER_SUCCESS(msg) printf("\033[32m%s\033[0m\n", msg)
#define STDOUT_LOGGER_WARNING(msg) fprintf(stderr, "\033[33m%s\033[0m\n", msg)
#define STDOUT_LOGGER_ERROR(msg) fprintf(stderr, "\033[31m%s\033[0m\n", msg)

typedef int log_level;

enum LogLevel {
    SUCCESS,
    INFO,
    WARNING,
    ERROR,
};

/**
 * @brief function to help with logging application status
 *
 * @param level Log level defined by the 'LogLevel' enum. (SUCCESS, ERROR, ...)
 * @param tag Optional tag to make debugging easier
 * @param msg Log message to be printed
 */
void logger(log_level level, char* tag, char* msg, ...);

#endif  // __MIMIR_LOG_H__
