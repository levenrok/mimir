#ifndef __MIMIR_LOG_H__
#define __MIMIR_LOG_H__

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "path.h"
#include "schema.h"

/**
 * @brief function to help with logging application status
 *
 * @param level Log level defined by the 'LogLevel' enum. (SUCCESS, ERROR, ...)
 * @param tag Optional tag to make debugging easier
 * @param print_stdout Print the log message to stdout or not
 * @param msg Log message to be printed
 */
void logStatus(log_level level, char* tag, bool print_stdout, char* msg, ...);

#endif  // __MIMIR_LOG_H__
