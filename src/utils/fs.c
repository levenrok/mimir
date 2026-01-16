#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "errs.h"
#include "include/fs.h"
#include "include/log.h"

Err getAppDataPath(char* path_buffer, char* filename) {
    const char* data_home = getenv("XDG_DATA_HOME");

    char base_path[256];

    if (data_home && data_home[0] != '\0') {
        snprintf(base_path, sizeof(base_path), "%s/mimir", data_home);
    } else {
        const char* home = getenv("HOME");

        snprintf(base_path, sizeof(base_path), "%s/.local/share/mimir", home);
    }

    Err rc = ensureDirectoryExists(base_path, true);
    if (rc == OK_FS_DIR_CREATE)
        logger(INFO, "data directory not found at '%s'. creating...", base_path);
    else if (rc == ERR_FS_DIR_NOT_EXIST || rc == ERR_FS_DATA_PATH_ACCESS)
        return ERR_FS_DATA_PATH_ACCESS;

    snprintf(path_buffer, sizeof(base_path) + 1, "%s/%s", base_path, filename);

    return OK;
}

Err ensureDirectoryExists(char* path, bool create_if_not_exist) {
    struct stat st = {0};

    Err ret = OK;

    int rc = stat(path, &st);
    if (rc == 0) {
        return ret;
    } else if (rc == -1) {
        if (create_if_not_exist) {
            ret = mkdir(path, 0700) == 0 ? OK_FS_DIR_CREATE : ERR_FS_DIR_CREATE;
            return ret;
        } else {
            ret = ERR_FS_DIR_NOT_EXIST;
            return ret;
        }
    }

    ret = ERR;
    return ret;
}
