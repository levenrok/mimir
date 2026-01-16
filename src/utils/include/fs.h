#ifndef __MIMIR_FS_H__
#define __MIMIR_FS_H__

#include <stdbool.h>

#include "errs.h"

Err getAppDataPath(char* path_buffer, char* filename);
Err ensureDirectoryExists(char* path, bool create_if_not_exist);

#endif  // __MIMIR_FS_H__
