#ifndef __MIMIR_ERR_H__
#define __MIMIR_ERR_H__

typedef int mimir_error;

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

#endif  // __MIMIR_ERR_H__
