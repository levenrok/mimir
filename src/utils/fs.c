#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#if defined(_WIN32)
#include <io.h>
#elif defined(__linux__) || defined(__APPLE__)
#include <unistd.h>
#endif

#include "include/fs.h"

PathErr ensureDirectoryExists(char* path, bool create_if_not_exist) {
#if defined(_WIN32)
    struct _stat st;

    int rc = _stat(path, &st);
#elif defined(__linux__) || defined(__APPLE__)
    struct stat st;

    int rc = stat(path, &st);
#endif
    if (rc == 0) {
        return PATH_OK;
    } else if (rc == -1 && errno == ENOENT) {
        if (create_if_not_exist) {
#if defined(_WIN32)
            if (_mkdir(path) == 0)
                return PATH_OK_DIR_CREATE;
            else
                return PATH_ERR_DIR_CREATE;
#elif defined(__linux__) || defined(__APPLE__)
            if (mkdir(path, 0700) == 0)
                return PATH_OK_DIR_CREATE;
            else
                return PATH_ERR_DIR_CREATE;
#endif
        } else {
            return PATH_ERR_DIR_NOT_EXIST;
        }
    }
    return PATH_ERR;
}

PathErr ensureFileExists(char* filepath, bool create_if_not_exist) {
#if defined(_WIN32)
    int rc = _access(filepath, F_OK);
#elif defined(__linux__) || defined(__APPLE__)
    int rc = access(filepath, F_OK);
#endif

    if (rc == 0) {
        return PATH_OK;
    } else if (rc == -1) {
        if (create_if_not_exist) {
            FILE* file = fopen(filepath, "w");
            if (file == NULL)
                return PATH_ERR_FILE_CREATE;

            fclose(file);
            return PATH_OK_FILE_CREATE;
        } else {
            return PATH_ERR_FILE_NOT_EXIST;
        }
    }

    return PATH_ERR;
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
