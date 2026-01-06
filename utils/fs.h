#ifndef __MIMIR_FS_H__
#define __MIMIR_FS_H__

#include <stdbool.h>

#include "err.h"

err_t getAppDataPath(char* path_buffer, char* filename);
err_t ensureDirectoryExists(char* path, bool create_if_not_exist);

#endif  // __MIMIR_FS_H__
