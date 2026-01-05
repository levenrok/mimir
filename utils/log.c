#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "fs.h"
#include "log.h"

#define LOG_SIZE 256

void logStatus(log_level level, char* tag, bool print_stdout, char* msg, ...) {
    char log_path[256];
    (void)getAppDataPath(log_path, "mimir.log");

    FILE* log_file = fopen(log_path, "a");

    time_t now;
    struct tm result;
    char buffer[32];

    time(&now);
    localtime_r(&now, &result);

    va_list args;
    va_start(args, msg);

    char log_format[LOG_SIZE * 2];
    char msg_format[LOG_SIZE];

    char* level_format = NULL;
    char* level_color = NULL;

    switch (level) {
        case SUCCESS:
            level_format = "SUCCESS";
            level_color = "\033[32m";
            break;
        case INFO:
            level_format = "INFO";
            level_color = "\033[34m";
            break;
        case WARNING:
            level_format = "WARNING";
            level_color = "\033[33m";
            break;
        case ERROR:
            level_format = "ERROR";
            level_color = "\033[31m";
            break;
    }

    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &result);

    vsnprintf(msg_format, sizeof(msg_format), msg, args);
    snprintf(log_format, sizeof(log_format), "[%s] %s (%s): %s\n", buffer, level_format, tag, msg_format);
    fputs(log_format, log_file);

    if (print_stdout) {
        fprintf(stderr, "%s%s\033[0m", level_color, msg_format);
    }

    va_end(args);

    fclose(log_file);
}
