#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "include/database.h"
#include "include/io.h"

#include "utils/include/log.h"

static const int CONTENT_SIZE = 1024;

err_t importScriptContent(char* buffer, int buffer_size, FILE** fp) {
    char template[] = "/tmp/script_XXXXXX";

    const char* editor = getenv("EDITOR") != NULL ? getenv("EDITOR") : "nano";
    int fd = 0;
    char command[256];

    char content[CONTENT_SIZE];

    err_t rc = OK;

    fd = mkstemp(template);
    if (fd == -1) {
        rc = ERR_IO_WRITE;
        goto err;
    }
    close(fd);

    snprintf(command, sizeof(command), "%s %s", editor, template);

    if (system(command) == -1) {
        logger(ERROR, "io", "failed to open temp the file with '%s'", editor);
        rc = ERR_IO_READ;
        goto err;
    }

    *fp = fopen(template, "r");
    if (fp == NULL) {
        rc = ERR_IO_READ;
        goto err;
    } else {
        size_t content_size = fread(content, 1, CONTENT_SIZE - 1, *fp);

        content[content_size] = '\0';

        fclose(*fp);
    }

    strncpy(buffer, content, buffer_size);

    if (remove(template) != 0) {
        logger(WARNING, "io", "failed to delete the temp file after retrieving contents");
    }

    return rc;

err:
    STDOUT_LOGGER_ERROR("%s", "cannot capture contents of the script! $_$");

    return rc;
}

err_t runScriptContent(sqlite3* db, char* name, FILE** fp) {
    char command[1024];
    char output[1024];

    getScript(db, name, command, false);

    *fp = popen(command, "r");
    if (*fp == NULL) {
        STDOUT_LOGGER_ERROR("cannot run the script '%s'! ~_~", name);

        logger(ERROR, "io", "failed to create pipe stream to run command '%s'", command);
        return ERR_IO_EXECUTE;
    }

    while (fgets(output, sizeof(output), *fp) != NULL) {
        printf("%s", output);
    }

    pclose(*fp);
    return OK;
}

void printHelp(char* program_name) {
    fprintf(stderr, "Usage:\n\t%s [name]\n\t%s [options] [name]\n\n", program_name, program_name);
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "\t-c, --create <name>\tCreate a new script\n");
    fprintf(stderr, "\t-b, --shebang <value>\tSpecify a shebang to the new script\n");
    fprintf(stderr, "\t-l, --list\t\tList all scripts\n");
    fprintf(stderr, "\t-s, --show <name>\tShow contents of the specified script\n");
}
