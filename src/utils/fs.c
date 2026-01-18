#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

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

Err ensureFileExists(char* filepath, bool create_if_not_exist) {
    int rc = access(filepath, F_OK);

    if (rc == 0) {
        return OK;
    } else if (rc == -1) {
        if (create_if_not_exist) {
            FILE* file = fopen(filepath, "w");
            if (file == NULL)
                return ERR_FS_FILE_CREATE;

            fclose(file);
            return OK_FS_FILE_CREATE;
        } else {
            return ERR_FS_FILE_NOT_EXIST;
        }
    }

    return ERR;
}

char* createTempFile(void) {
    char template[] = "/tmp/script_XXXXXX";
    int fd = 0;

    size_t src_len = sizeof(template);

    fd = mkstemp(template);
    if (fd == -1)
        return NULL;
    close(fd);

    char* temp = malloc(src_len);
    if (temp == NULL)
        return NULL;

    (void)strncpy(temp, template, src_len);

    return temp;
}

void deleteTempFile(char** temp) {
    if (temp != NULL && *temp != NULL) {
        (void)unlink(*temp);

        free(*temp);
        *temp = NULL;
    }
}
