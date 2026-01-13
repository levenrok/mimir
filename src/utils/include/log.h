#ifndef __MIMIR_LOG_H__
#define __MIMIR_LOG_H__

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

#define CLR_RED "\033[31m"
#define CLR_GRN "\033[32m"
#define CLR_YLW "\033[33m"
#define CLR_RST "\033[0m"

#define STDOUT_LOGGER_SUCCESS(fmt, ...) printf(CLR_GRN fmt CLR_RST "\n", __VA_ARGS__)
#define STDOUT_LOGGER_WARNING(fmt, ...) fprintf(stderr, CLR_YLW fmt CLR_RST "\n", __VA_ARGS__)
#define STDOUT_LOGGER_ERROR(fmt, ...) fprintf(stderr, CLR_RED fmt CLR_RST "\n", __VA_ARGS__)

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
