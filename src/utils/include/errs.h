#ifndef __MIMIR_ERRS_H__
#define __MIMIR_ERRS_H__

#include <sqlite3.h>
#include <stdbool.h>
#include <stdio.h>

#define ERR_INFO                                                                                   \
    X_ERR(OK, "app", "success ^_^", false)                                                         \
    X_ERR(ERR, "app", "unexpected error occured x_x", true)                                        \
    X_ERR(OK_FS_DIR_CREATE, "fs", "directory created successfully", false)                         \
    X_ERR(ERR_FS_DIR_CREATE, "fs", "failed to create directory", false)                            \
    X_ERR(ERR_FS_DIR_NOT_EXIST, "fs", "failed to find directory", false)                           \
    X_ERR(ERR_FS_DATA_PATH_ACCESS, "fs", "failed to access data path", true)                       \
    X_ERR(ERR_IO_READ, "io", "failed to read file", true)                                          \
    X_ERR(ERR_IO_WRITE, "io", "failed to write file", true)                                        \
    X_ERR(ERR_IO_EXECUTE, "io", "failed to execute command", true)                                 \
    X_ERR(ERR_DB_ACCESS, "db", "failed to access the database", true)                              \
    X_ERR(ERR_DB_OPEN, "db", "failed to open the connection to the database", true)                \
    X_ERR(ERR_DB_CREATE_TABLE, "db", "failed to create database table", true)                      \
    X_ERR(ERR_DB_INSERT, "db", "failed to save record(s) to the database", true)                   \
    X_ERR(ERR_DB_INSERT_UNIQUE, "db", "failed to save duplicate record(s) to the database", true)  \
    X_ERR(ERR_DB_SELECT, "db", "failed to retrieve record(s) from the database", true)             \
    X_ERR(ERR_DB_SELECT_NO_MATCH, "db", "failed to find matching record(s) in the database", true) \
    X_ERR(ERR_DB_DELETE, "db", "failed to delete record(s) from the database", true)               \
    X_ERR(ERR_DB_DELETE_NO_MATCH, "db", "failed to find matching record(s) in the database to delete", true)

#define ERR_INFO_COUNT 18

#define X_ERR(code, tag, desc, exit) code,
typedef enum Err { ERR_INFO } Err;

void handle_err(Err action, sqlite3** db, FILE** fp);

#endif  // __MIMIR_ERRS_H__
