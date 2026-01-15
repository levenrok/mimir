#include <string.h>
#include <time.h>
#include <unistd.h>

#include "include/fs.h"
#include "include/log.h"

static const int MSG_SIZE = 256;

#undef Y
#define Y(level) #level,
static const char* log_level_names[] = {LOG_LEVEL};

void logger(LogLevel level, char* tag, char* fmt, ...) {
    char log_path[256];
    if (getAppDataPath(log_path, "mimir.log") != OK) {
        STDOUT_LOGGER_WARNING("%s", "cannot access the log file. stopping logger...");
        return;
    }

    FILE* log_file = fopen(log_path, "a");

    time_t now;
    struct tm result;
    char current_time[32];

    time(&now);
    localtime_r(&now, &result);
    strftime(current_time, sizeof(current_time), "%Y-%m-%d %H:%M:%S", &result);

    char log_msg[MSG_SIZE];
    const char* log_level = log_level_names[level];
    const size_t LOG_SIZE = strlen(current_time) + strlen(log_level) + MSG_SIZE + 16;
    char log[LOG_SIZE];

    va_list args;
    va_start(args, fmt);

    vsnprintf(log_msg, sizeof(log_msg), fmt, args);

    snprintf(log, LOG_SIZE, "[%s] %s (%s): %s\n", current_time, log_level, tag, log_msg);
    fputs(log, log_file);

    va_end(args);
    fclose(log_file);
}

void stdout_logger(LogLevel level, char* fmt, ...) {
    char log_msg[MSG_SIZE];
    const char* log_level = log_level_names[level];

    va_list args;
    va_start(args, fmt);

    vsnprintf(log_msg, sizeof(log_msg), fmt, args);

    const char* log_clr = NULL;
    const char* log_txt_rst = TXT_RST;
    size_t log_size = 0;
    const int log_padding = 2;

    switch (level) {
        case SUCCESS:
        case INFO:
            log_size = strlen(log_msg) + log_padding;
            log_clr = level == SUCCESS ? CLR_GRN : CLR_BLU;

            printf("%s┌", log_clr);
            for (int i = 0; i < log_size; i++) {
                printf("─");
            }
            printf("┐%s\n", log_txt_rst);

            printf("%s│ %s │%s\n", log_clr, log_msg, log_txt_rst);

            printf("%s└", log_clr);
            for (int i = 0; i < log_size; i++) {
                printf("─");
            }
            printf("┘%s\n", log_txt_rst);

            break;
        case WARNING:
        case ERROR:
            log_size = strlen(log_level) + 2 + strlen(log_msg) + log_padding;
            log_clr = level == ERROR ? CLR_RED : CLR_YLW;

            fprintf(stderr, "%s┌", log_clr);
            for (int i = 0; i < log_size; i++) {
                fprintf(stderr, "─");
            }
            fprintf(stderr, "┐%s\n", log_txt_rst);

            fprintf(stderr,
                    "%s│ " TXT_BLD "%s:" TXT_RST
                    "%s %s"
                    "%s │%s\n",
                    log_clr, log_level, log_clr, log_msg, log_clr, log_txt_rst);

            fprintf(stderr, "%s└", log_clr);
            for (int i = 0; i < log_size; i++) {
                fprintf(stderr, "─");
            }
            fprintf(stderr, "┘%s\n", log_txt_rst);

            break;
    }

    va_end(args);
}
