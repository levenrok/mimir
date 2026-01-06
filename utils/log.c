#include <time.h>
#include <unistd.h>

#include "fs.h"
#include "log.h"

#define LOG_SIZE 256

void logger(log_level level, char* tag, char* msg, ...) {
    char log_path[256];
    if (getAppDataPath(log_path, "mimir.log") != OK) {
        STDOUT_LOGGER_WARNING("cannot access the log file. aborting logging...");
        return;
    }

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

    switch (level) {
        case SUCCESS:
            level_format = "SUCCESS";
            break;
        case INFO:
            level_format = "INFO";
            break;
        case WARNING:
            level_format = "WARNING";
            break;
        case ERROR:
            level_format = "ERROR";
            break;
    }

    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &result);

    vsnprintf(msg_format, sizeof(msg_format), msg, args);
    snprintf(log_format, sizeof(log_format), "[%s] %s (%s): %s\n", buffer, level_format, tag, msg_format);
    fputs(log_format, log_file);

    va_end(args);

    fclose(log_file);
}
