#ifndef __MIMIR_ERR_H__
#define __MIMIR_ERR_H__

#include <sqlite3.h>
#include <stdbool.h>
#include <stdio.h>

#define ERR_INFO                                                               \
    X(OK, "success ^_^", false)                                                \
    X(ERR, "unexpected error occured x_x", true)                               \
    X(OK_DIR_CREATE, "directory created successfully.", false)                 \
    X(ERR_FS_DIR_NOT_EXIST, "cannot find the directory!", false)               \
    X(ERR_FS_DATA_PATH_ACCESS, "failed to access data path!", false)           \
    X(ERR_IO_READ, "failed to read file!", true)                               \
    X(ERR_IO_WRITE, "failed to write file!", true)                             \
    X(ERR_IO_EXECUTE, "failed to execute command!", true)                      \
    X(ERR_DB_OPEN, "failed to open the database!", true)                       \
    X(ERR_DB_INSERT, "failed to write to the database!", true)                 \
    X(ERR_DB_INSERT_UNIQUE, "script with the same name already exists!", true) \
    X(ERR_DB_SELECT, "failed to read from the database!", true)

#define ERR_INFO_COUNT 12

typedef int err_t;

#define X(code, desc, exit) code,
enum ErrCode { ERR_INFO };

void _err_handler(err_t action, sqlite3** db, FILE** fp);

#endif  // __MIMIR_ERR_H__
