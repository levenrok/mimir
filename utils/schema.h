#ifndef __MIMIR_UTIL_H__
#define __MIMIR_UTIL_H__

typedef int mimir_error;
typedef int log_level;

typedef struct {
    char name[128];
    char contents[1024];
    char shebang[32];
} ScriptInfo;

enum ErrorCode {
    OK,
    NOT_OK,
    CLI_INVALID_ARG,
    CLI_INVALID_OPT,
    IO_READ_ERROR,
    IO_WRITE_ERROR,
    IO_EXECUTE_ERROR,
    DB_OPEN_ERROR,
    DB_INSERT_ERROR,
    DB_SELECT_ERROR,
    PATH_MISSING_ERROR,
};

enum LogLevel {
    SUCCESS,
    INFO,
    WARNING,
    ERROR,
};

#endif  // __MIMIR_UTIL_H__
