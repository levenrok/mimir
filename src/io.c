#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "include/io.h"

#include "utils/include/fs.h"
#include "utils/include/log.h"

#ifndef CONTENT_SIZE
#define CONTENT_SIZE 1024
#endif

IoErr importScriptContent(char* buffer, int buffer_size, FILE** fp) {
    const char* editor = getenv("EDITOR") != NULL ? getenv("EDITOR") : "nano";
    char command[256];

    char content[CONTENT_SIZE];

    IoErr rc = IO_OK;

    char* temp = createTempFile();
    if (temp == NULL) {
        rc = IO_ERR_WRITE;
        goto err;
    }

    snprintf(command, sizeof(command), "%s %s", editor, temp);

    if (system(command) == -1) {
        rc = IO_ERR_READ;
        goto err;
    }

    *fp = fopen(temp, "r");
    if (fp == NULL) {
        rc = IO_ERR_READ;
        goto err;
    } else {
        size_t content_size = fread(content, 1, CONTENT_SIZE - 1, *fp);

        content[content_size] = '\0';

        fclose(*fp);
    }

    strncpy(buffer, content, buffer_size);

    deleteTempFile(&temp);
    return rc;

err:
    STDOUT_LOGGER_ERROR("%s", "cannot capture contents of the script");

    deleteTempFile(&temp);
    return rc;
}

IoErr runScriptContent(Database* db, char* name, FILE** fp) {
    char command[1024];
    char output[1024];

    if (getScriptContent(db, name, command, false) != DB_OK)
        goto err;

    *fp = popen(command, "r");
    if (*fp == NULL) {
        STDOUT_LOGGER_ERROR("cannot run the script '%s'", name);
        goto err;
    }

    while (fgets(output, sizeof(output), *fp) != NULL) {
        printf("%s", output);
    }

    pclose(*fp);
    return IO_OK;

err:
    return IO_ERR_EXECUTE;
}

void printMan(char* program_name) {
    if (system("man mimir") != 0)
        printHelp(program_name);
}

void printHelp(char* program_name) {
    fprintf(stderr, "Usage:\n\t%s [name]\n\t%s [options] [name]\n\n", program_name, program_name);
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "\t-c, --create <name>\tCreate a new script\n");
    fprintf(stderr, "\t-b, --shebang <value>\tSpecify a shebang to the new script\n");
    fprintf(stderr, "\t-l, --list\t\tList all scripts\n");
    fprintf(stderr, "\t-s, --show <name>\tShow contents of the specified script\n");
    fprintf(stderr, "\t-d, --delete <name>\tDelete the specified script\n");
    fprintf(stderr, "\t-h, --help\t\tPrint the help output\n");
}
