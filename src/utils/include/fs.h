#ifndef __MIMIR_FS_H__
#define __MIMIR_FS_H__

#include <stdbool.h>

#include "errs.h"

Err getAppDataPath(char* path_buffer, char* filename);
Err ensureDirectoryExists(char* path, bool create_if_not_exist);
Err ensureFileExists(char* filepath, bool create_if_not_exist);
char* createTempFile(void);
void deleteTempFile(char** temp);

#endif  // __MIMIR_FS_H__
