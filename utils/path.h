#ifndef __MIMIR_PATH_H__
#define __MIMIR_PATH_H__

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "schema.h"

mimir_error getAppDataPath(char* path_buffer, char* filename);
mimir_error ensureDirectoryExists(char* path, bool create_if_not_exist);

#endif
