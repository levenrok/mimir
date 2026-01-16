#include <getopt.h>
#include <sqlite3.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "src/include/database.h"
#include "src/include/io.h"

#include "src/utils/include/errs.h"
#include "src/utils/include/log.h"

/**
 * Main Entrypoint
 */
int main(int argc, char* argv[]) {
    size_t buf_size = 0;
    bool insert_flag = false;
    int opt = 0;

    sqlite3* db = NULL;
    FILE* fp = NULL;

    handle_err(openDatabase(&db), &db, &fp);

    struct option cli_options[] = {
        {"create", required_argument, NULL, 'c'}, {"shebang", required_argument, NULL, 'b'},
        {"show", required_argument, NULL, 's'},   {"list", no_argument, NULL, 'l'},
        {"help", no_argument, NULL, 'h'},         {NULL, 0, NULL, 0},
    };

    ScriptInfo script = {0};

    if (argc < 2) {
        goto err_arg;
    }

    while ((opt = getopt_long(argc, argv, "c:b:ls:h", cli_options, NULL)) != -1) {
        switch (opt) {
            case 'c':
                buf_size = sizeof(char) * 128;
                strncpy(script.name, optarg, buf_size);
                script.name[buf_size - 1] = '\0';

                handle_err(importScriptContent(script.contents, 1024, &fp), &db, &fp);
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
                handle_err(getScripts(db), &db, &fp);

                insert_flag = false;
                break;
            case 's':
                handle_err(getScriptContent(db, optarg, NULL, true), &db, &fp);

                insert_flag = false;
                break;
            case 'h':
                printMan(argv[0]);
                break;
            default:
                goto err_opt;
        }
    }

    if (insert_flag) {
        handle_err(insertScript(db, script.name, script.contents, script.shebang), &db, &fp);
    }

    for (int i = optind; i < argc; i++) {
        buf_size = sizeof(char) * 128;
        strncpy(script.name, argv[i], buf_size);
        script.name[buf_size - 1] = '\0';

        handle_err(runScriptContent(db, script.name, &fp), &db, &fp);
    }

    closeDatabase(&db);
    return OK;

err_arg:
    closeDatabase(&db);
    printHelp(argv[0]);
    return ERR;
err_opt:
    closeDatabase(&db);
    printHelp(argv[0]);
    return ERR;
}
