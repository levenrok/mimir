#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "path.h"

mimir_error getAppDataPath(char* path_buffer, char* filename) {
    const char* data_home = getenv("XDG_DATA_HOME");

    char base_path[256];

    if (data_home && data_home[0] != '\0') {
        snprintf(base_path, sizeof(base_path), "%s/mimir", data_home);
    } else {
        const char* home = getenv("HOME");

        snprintf(base_path, sizeof(base_path), "%s/.local/share/mimir", home);
    }

    (void)ensureDirectoryExists(base_path, true);

    snprintf(path_buffer, sizeof(base_path), "%s/%s", base_path, filename);

    return OK;
}

mimir_error ensureDirectoryExists(char* path, bool create_if_not_exist) {
    struct stat st = {0};

    int rc = stat(path, &st);
    if (rc == 0) {
        return OK;
    } else if (rc == -1) {
        if (create_if_not_exist)
            mkdir(path, 0700);
        else
            return PATH_MISSING_ERROR;
    }

    return NOT_OK;
}
