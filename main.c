#include <getopt.h>
#include <sqlite3.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "database.h"
#include "err.h"
#include "io.h"

/**
 * Main Entrypoint
 */
int main(int argc, char* argv[]) {
    size_t buf_size = 0;
    bool insert_flag = false;
    int opt = 0;

    sqlite3* db = NULL;

    openDatabase(&db);

    struct option cli_options[] = {
        {"create", required_argument, NULL, 'c'},
        {"shebang", required_argument, NULL, 'b'},
        {"show", required_argument, NULL, 's'},
        {"list", no_argument, NULL, 'l'},
        {NULL, 0, NULL, 0},
    };

    ScriptInfo script;

    if (argc < 2) {
        goto err_arg;
    }

    while ((opt = getopt_long(argc, argv, "c:b:s:le:", cli_options, NULL)) != -1) {
        switch (opt) {
            case 'c':
                buf_size = sizeof(char) * 128;
                strncpy(script.name, optarg, buf_size);
                script.name[buf_size - 1] = '\0';

                importScriptContent(script.contents, 1024);
                insert_flag = true;
                break;
            case 'b':
                buf_size = sizeof(char) * 32;
                strncpy(script.shebang, optarg, buf_size);
                script.shebang[buf_size - 1] = '\0';

                // Prevents the user from specifying a 'shebang' if the 'create' flag is not used
                if (!insert_flag) {
                    fprintf(stderr, "\033[31mOnly specify a shebang when creating a new script!\033[0m\n");
                    goto err_arg;
                }

                insert_flag = true;
                break;
            case 's':
                (void)getScript(db, optarg, NULL, true);

                insert_flag = false;
                break;
            case 'l':
                (void)getScripts(db);

                insert_flag = false;
                break;
            case 'e':
                insert_flag = false;
                break;
            default:
                goto err_opt;
        }
    }

    if (insert_flag && (insertScript(db, script.name, script.contents, script.shebang) != OK)) {
        return DB_INSERT_ERROR;
    }

    for (int i = optind; i < argc; i++) {
        buf_size = sizeof(char) * 128;
        strncpy(script.name, argv[i], buf_size);
        script.name[buf_size - 1] = '\0';

        runScriptContent(db, script.name);
    }

    closeDatabase(&db);
    return OK;

err_arg:
    closeDatabase(&db);
    printHelp(argv[0]);
    return CLI_INVALID_ARG;
err_opt:
    closeDatabase(&db);
    printHelp(argv[0]);
    return CLI_INVALID_OPT;
}
