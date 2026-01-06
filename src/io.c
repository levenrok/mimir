#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "database.h"
#include "io.h"
#include "log.h"

#define CONTENT_SIZE 1024

err_t importScriptContent(char* buffer, int buffer_size, FILE** fp) {
    char template[] = "/tmp/script_XXXXXX";

    const char* editor = getenv("EDITOR") != NULL ? getenv("EDITOR") : "nano";
    int fd = 0;
    char command[256];

    char content[CONTENT_SIZE];

    err_t rc = OK;

    fd = mkstemp(template);
    if (fd == -1) {
        logger(ERROR, "io", "could not create the temp file to get script contents");
        rc = ERR_IO_WRITE;
        goto err;
    }
    close(fd);

    logger(INFO, "io", "opening temp file with '%s'", editor);
    snprintf(command, sizeof(command), "%s %s", editor, template);

    if (system(command) == -1) {
        logger(ERROR, "io", "could not open temp the file with 'editor'");
        rc = ERR_IO_READ;
        goto err;
    }

    *fp = fopen(template, "r");
    if (fp == NULL) {
        logger(ERROR, "io", "could not open temp the file to get script contents");
        rc = ERR_IO_READ;
        goto err;
    } else {
        size_t content_size = fread(content, 1, CONTENT_SIZE - 1, *fp);

        content[content_size] = '\0';

        fclose(*fp);
    }

    strncpy(buffer, content, buffer_size);

    if (remove(template) != 0) {
        logger(WARNING, "io", "failed to remove temp file :|");
    }

    logger(SUCCESS, "io", "got the script contents sucessfully!");
    return rc;

err:
    return rc;
}

err_t runScriptContent(sqlite3* db, char* name, FILE** fp) {
    char command[1024];
    char output[1024];

    getScript(db, name, command, false);

    *fp = popen(command, "r");
    if (*fp == NULL) {
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
