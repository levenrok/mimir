#ifndef __MIMIR_LOG_H__
#define __MIMIR_LOG_H__

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

#define CLR_RED "\033[31m"
#define CLR_GRN "\033[32m"
#define CLR_YLW "\033[33m"
#define CLR_BLU "\033[34m"
#define TXT_BLD "\033[1m"
#define TXT_RST "\033[0m"

#define STDOUT_LOGGER_SUCCESS(fmt, ...) stdout_logger(SUCCESS, fmt, __VA_ARGS__)
#define STDOUT_LOGGER_INFO(fmt, ...) stdout_logger(INFO, fmt, __VA_ARGS__)
#define STDOUT_LOGGER_WARNING(fmt, ...) stdout_logger(WARNING, fmt, __VA_ARGS__)
#define STDOUT_LOGGER_ERROR(fmt, ...) stdout_logger(ERROR, fmt, __VA_ARGS__)
#define STDOUT_LOGGER_DEBUG(msg, code) stdout_debug(msg, code, __FILE_NAME__, __LINE__)

#define LOG_LEVEL \
    Y(SUCCESS)    \
    Y(INFO)       \
    Y(WARNING)    \
    Y(ERROR)

#define Y(level) level,
typedef enum LogLevel { LOG_LEVEL } LogLevel;

/**
 * @brief function to help with logging application status
 *
 * @param level Log level defined by the 'LogLevel' enum. (SUCCESS, ERROR, ...)
 * @param tag Optional tag to make debugging easier
 * @param msg Log message to be printed
 */
void logger(LogLevel level, char* tag, char* fmt, ...);

void stdout_logger(LogLevel level, char* fmt, ...);
void stdout_debug(const char* msg, const char* code, const char* filename, const int line);

#endif  // __MIMIR_LOG_H__
