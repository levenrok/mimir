#ifndef __MIMIR_ERR_H__
#define __MIMIR_ERR_H__

#include <sqlite3.h>
#include <stdbool.h>
#include <stdio.h>

#define ERR_INFO                                                                              \
    X(OK, "app", "success ^_^", false)                                                        \
    X(ERR, "app", "unexpected error occured x_x", true)                                       \
    X(OK_FS_DIR_CREATE, "fs", "directory created successfully", false)                        \
    X(ERR_FS_DIR_CREATE, "fs", "failed to create directory", false)                           \
    X(ERR_FS_DIR_NOT_EXIST, "fs", "failed to find directory", false)                          \
    X(ERR_FS_DATA_PATH_ACCESS, "fs", "failed to access data path", true)                      \
    X(ERR_IO_READ, "io", "failed to read file", true)                                         \
    X(ERR_IO_WRITE, "io", "failed to write file", true)                                       \
    X(ERR_IO_EXECUTE, "io", "failed to execute command", true)                                \
    X(ERR_DB_ACCESS, "db", "failed to access the database", true)                             \
    X(ERR_DB_OPEN, "db", "failed to open the connection to the database", true)               \
    X(ERR_DB_CREATE_TABLE, "db", "failed to create database table", true)                     \
    X(ERR_DB_INSERT, "db", "failed to save record(s) to the database", true)                  \
    X(ERR_DB_INSERT_UNIQUE, "db", "failed to save duplicate record(s) to the database", true) \
    X(ERR_DB_SELECT, "db", "failed to retrieve record(s) from the database", true)

#define ERR_INFO_COUNT 15

typedef int err_t;

#define X(code, tag, desc, exit) code,
enum ErrCode { ERR_INFO };

void handle_err(err_t action, sqlite3** db, FILE** fp);

#endif  // __MIMIR_ERR_H__
