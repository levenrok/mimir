#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "database.h"
#include "io.h"
#include "log.h"

#define CONTENT_SIZE 1024

mimir_error importScriptContent(char* buffer, int buffer_size) {
    char template[] = "/tmp/script_XXXXXX";

    const char* editor = getenv("EDITOR") != NULL ? getenv("EDITOR") : "nano";
    int fd = 0;
    char command[256];

    FILE* fp;
    char content[CONTENT_SIZE];

    mimir_error rc = OK;

    fd = mkstemp(template);
    if (fd == -1) {
        logStatus(ERROR, "io", true, "could not create the temp file to get script contents :(");
        rc = IO_WRITE_ERROR;
        goto err;
    }
    close(fd);

    logStatus(INFO, "io", false, "opening temp file with '%s'", editor);
    snprintf(command, sizeof(command), "%s %s", editor, template);

    if (system(command) == -1) {
        logStatus(ERROR, "io", true, "could not open temp the file with 'editor' :(");
        rc = IO_READ_ERROR;
        goto err;
    }

    fp = fopen(template, "r");
    if (fp == NULL) {
        logStatus(ERROR, "io", true, "could not open temp the file to get script contents :(");
        rc = IO_READ_ERROR;
        goto err;
    } else {
        size_t content_size = fread(content, 1, CONTENT_SIZE - 1, fp);

        content[content_size] = '\0';

        fclose(fp);
    }

    strncpy(buffer, content, buffer_size);

    if (remove(template) != 0) {
        logStatus(WARNING, "io", false, "failed to remove temp file :|");
    }

    logStatus(SUCCESS, "io", false, "got the script contents sucessfully! :)");
    return rc;

err:
    return rc;
}

mimir_error runScriptContent(sqlite3* db, char* name) {
    FILE* sh;

    char command[1024];
    char output[1024];

    getScript(db, name, command, false);

    sh = popen(command, "r");
    if (sh == NULL) {
        return IO_EXECUTE_ERROR;
    }

    while (fgets(output, sizeof(output), sh) != NULL) {
        printf("%s", output);
    }

    pclose(sh);
    return OK;
}

void printHelp(char* program_name) {
    fprintf(stderr, "Usage:\n\t%s [name]\n\t%s [options] [name]\n\n", program_name, program_name);
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "\t-c, --create <name>\tCreate a new script\n");
    fprintf(stderr, "\t-b, --shebang <name>\tSpecify a shebang to the new script\n");
    fprintf(stderr, "\t-l, --list\tList all scripts\n");
    fprintf(stderr, "\t-s, --show\tShow contents of the specified script\n");
}
