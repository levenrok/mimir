#include <getopt.h>
#include <sqlite3.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"

#include "include/database.h"
#include "include/io.h"

#include "utils/include/fs.h"
#include "utils/include/log.h"

Info bootloader(void);

/**
 * Main Entrypoint
 */
int main(int argc, char* argv[]) {
    size_t buf_size = 0;
    bool insert_flag = false;
    int opt = 0;

    const Info info = bootloader();

#if defined(__GNUC__) || defined(__clang__)
    __attribute__((cleanup(closeDatabase))) Database db;
#else
    Database db;
#endif

    FILE* fp = NULL;

    openDatabase(&db, info.database_path, info.log_path);
    initDatabase(&db, info.log_path);

    struct option cli_options[] = {
        {"create", required_argument, NULL, 'c'},
        {"shebang", required_argument, NULL, 'b'},
        {"show", required_argument, NULL, 's'},
        {"list", no_argument, NULL, 'l'},
        {"delete", required_argument, NULL, 'd'},
        {"help", no_argument, NULL, 'h'},
        {NULL, 0, NULL, 0},
    };

    Script script = {0};

    if (argc < 2) {
        goto err_arg;
    }

    while ((opt = getopt_long(argc, argv, "c:b:ls:d:h", cli_options, NULL)) != -1) {
        switch (opt) {
            case 'c':
                buf_size = sizeof(char) * 128;
                strncpy(script.name, optarg, buf_size);
                script.name[buf_size - 1] = '\0';

                importScriptContent(script.contents, 1024, &fp);
                insert_flag = true;
                break;
            case 'b':
                buf_size = sizeof(char) * 32;
                strncpy(script.shebang, optarg, buf_size);
                script.shebang[buf_size - 1] = '\0';

                // Prevents the user from specifying a 'shebang' if the 'create' flag is not used
                if (!insert_flag) {
                    STDOUT_LOGGER_ERROR("%s", "Only specify a shebang when creating a new script!");
                    goto err_arg;
                }

                insert_flag = true;
                break;
            case 'l':
                getScripts(&db);

                insert_flag = false;
                break;
            case 's':
                getScriptContent(&db, optarg, NULL, true);

                insert_flag = false;
                break;
            case 'd':
                buf_size = sizeof(char) * 128;
                strncpy(script.name, optarg, buf_size);
                script.name[buf_size - 1] = '\0';

                deleteScript(&db, script.name);
                break;
            case 'h':
                printMan(argv[0]);
                break;
            default:
                goto err_opt;
        }
    }

    if (insert_flag) {
        insertScript(&db, script.name, script.contents, script.shebang);
    }

    for (int i = optind; i < argc; i++) {
        buf_size = sizeof(char) * 128;
        strncpy(script.name, argv[i], buf_size);
        script.name[buf_size - 1] = '\0';

        runScriptContent(&db, script.name, &fp);
    }

#if !defined(__GNUC__) || !defined(__clang__)
    closeDatabase(&db);
#endif

    return 0;

err_arg:
#if !defined(__GNUC__) || !defined(__clang__)
    closeDatabase(&db);
#endif

    printHelp(argv[0]);
    return 1;

err_opt:
#if !defined(__GNUC__) || !defined(__clang__)
    closeDatabase(&db);
#endif

    printHelp(argv[0]);
    return 1;
}

Info bootloader(void) {
    char* base_path = getenv("XDG_DATA_HOME");

    char data_path[4096];
    char database_path[4096];
    char log_path[4096];

    if (!base_path) {
        base_path = getenv("HOME");

#if defined(__linux__)
        size_t data_path_size =
            strlen(base_path) + strlen("/.local/share/") + strlen(MIMIR_APPLICATION_NAME) + 1;  // '\0'
        snprintf(data_path, data_path_size, "%s/.local/share/%s", base_path, MIMIR_APPLICATION_NAME);
#elif defined(__APPLE__)
        size_t data_path_size =
            strlen(base_path) + strlen("/Library/Application Support/") + strlen(MIMIR_APPLICATION_NAME) + 1;  // '\0'
        snprintf(data_path, data_path_size, "%s/Library/Application Support/%s", base_path, MIMIR_APPLICATION_NAME);
#endif
    }

    size_t data_path_size = strlen(base_path) + strlen(MIMIR_APPLICATION_NAME) + 2;  // '/' + '\0'
    snprintf(data_path, data_path_size, "%s/%s", base_path, MIMIR_APPLICATION_NAME);

    PathErr rc = ensureDirectoryExists(data_path, true);
    switch (rc) {
        case PATH_OK:
        case PATH_OK_DIR_CREATE:
            break;
        case PATH_ERR:
        case PATH_ERR_DIR_CREATE:
            goto err;
        default:
            goto err;
    }

    size_t database_path_size = strlen(data_path) + strlen("db.sqlite3") + 2;  // '/' + '\0'
    snprintf(database_path, database_path_size, "%s/%s", data_path, "db.sqlite3");

    rc = ensureFileExists(database_path, true);
    switch (rc) {
        case PATH_OK:
        case PATH_OK_FILE_CREATE:
            break;
        case PATH_ERR:
        case PATH_ERR_FILE_CREATE:
            goto err;
        default:
            goto err;
    }

    size_t log_path_size = strlen(data_path) + strlen("mimir.log") + 2;  // '/' + '\0'
    snprintf(log_path, log_path_size, "%s/%s", data_path, "mimir.log");

    rc = ensureFileExists(log_path, true);
    switch (rc) {
        case PATH_OK:
        case PATH_OK_FILE_CREATE:
            break;
        case PATH_ERR:
        case PATH_ERR_FILE_CREATE:
            goto err;
        default:
            goto err;
    }

    return (Info){
        .base_path = base_path,
        .database_path = database_path,
        .log_path = log_path,
    };

err:
    return (Info){
        .base_path = NULL,
        .database_path = NULL,
        .log_path = NULL,
    };
}
