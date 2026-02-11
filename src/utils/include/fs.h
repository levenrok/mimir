#ifndef MIMIR_FS_H
#define MIMIR_FS_H

#include <stdbool.h>

typedef enum PathErr {
    PATH_OK,
    PATH_ERR,
    PATH_OK_DIR_CREATE,
    PATH_OK_FILE_CREATE,
    PATH_ERR_DIR_CREATE,
    PATH_ERR_FILE_CREATE,
    PATH_ERR_DIR_NOT_EXIST,
    PATH_ERR_FILE_NOT_EXIST,
} PathErr;

PathErr ensureDirectoryExists(char* path, bool create_if_not_exist);
PathErr ensureFileExists(char* filepath, bool create_if_not_exist);
char* createTempFile(void);
void deleteTempFile(char** temp);

#endif  // MIMIR_FS_H
