#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "include/database.h"
#include "include/io.h"

#include "utils/include/fs.h"
#include "utils/include/log.h"

static const int CONTENT_SIZE = 1024;

Err importScriptContent(char* buffer, int buffer_size, FILE** fp) {
    const char* editor = getenv("EDITOR") != NULL ? getenv("EDITOR") : "nano";
    char command[256];

    char content[CONTENT_SIZE];

    Err rc = OK;

    char* temp = createTempFile();
    if (temp == NULL) {
        rc = ERR_IO_WRITE;
        goto err;
    }

    snprintf(command, sizeof(command), "%s %s", editor, temp);

    if (system(command) == -1) {
        logger(ERROR, "io", "failed to open temp the file with '%s'", editor);
        rc = ERR_IO_READ;
        goto err;
    }

    *fp = fopen(temp, "r");
    if (fp == NULL) {
        rc = ERR_IO_READ;
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

Err runScriptContent(sqlite3* db, char* name, FILE** fp) {
    char command[1024];
    char output[1024];

    handle_err(getScriptContent(db, name, command, false), &db, fp);

    *fp = popen(command, "r");
    if (*fp == NULL) {
        STDOUT_LOGGER_ERROR("cannot run the script '%s'", name);

        logger(ERROR, "io", "failed to create pipe stream to run command '%s'", command);
        return ERR_IO_EXECUTE;
    }

    while (fgets(output, sizeof(output), *fp) != NULL) {
        printf("%s", output);
    }

    pclose(*fp);
    return OK;
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
