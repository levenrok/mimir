#ifndef __MIMIR_PATH_H__
#define __MIMIR_PATH_H__

#include <stdbool.h>

#include "err.h"

mimir_error getAppDataPath(char* path_buffer, char* filename);
mimir_error ensureDirectoryExists(char* path, bool create_if_not_exist);

#endif
