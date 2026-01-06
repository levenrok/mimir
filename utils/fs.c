#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "err.h"

#include "fs.h"

err_t getAppDataPath(char* path_buffer, char* filename) {
    const char* data_home = getenv("XDG_DATA_HOME");

    char base_path[256];

    if (data_home && data_home[0] != '\0') {
        snprintf(base_path, sizeof(base_path), "%s/mimir", data_home);
    } else {
        const char* home = getenv("HOME");

        snprintf(base_path, sizeof(base_path), "%s/.local/share/mimir", home);
    }

    err_t rc = ensureDirectoryExists(base_path, true);
    if (rc != OK) {
        switch (rc) {
            case OK_DIR_CREATE:
                break;
            case ERR_FS_DIR_NOT_EXIST:
            case ERR:
                return ERR_FS_DATA_PATH_ACCESS;
        }
    }

    snprintf(path_buffer, sizeof(base_path) + 1, "%s/%s", base_path, filename);

    return OK;
}

err_t ensureDirectoryExists(char* path, bool create_if_not_exist) {
    struct stat st = {0};

    int rc = stat(path, &st);
    if (rc == 0) {
        return OK;
    } else if (rc == -1) {
        if (create_if_not_exist) {
            mkdir(path, 0700);
            return OK_DIR_CREATE;
        } else
            return ERR_FS_DIR_NOT_EXIST;
    }

    return ERR;
}
