#include <stdbool.h>
#include <stdlib.h>

#include "err.h"
#include "log.h"

#undef X
#define X(code, desc, exit) {code, desc, exit},

typedef struct ErrInfo {
    err_t err_code;
    char* err_desc;
    bool exit_on_err;
} ErrInfo;

static ErrInfo err_table[] = {ERR_INFO};

void _err_handler(err_t action, sqlite3** db, FILE** fp) {
    if (action == OK)
        return;

    err_t code = ERR;
    char* desc = NULL;
    bool should_exit = true;

    ErrInfo* end = err_table + ERR_INFO_COUNT;
    for (ErrInfo* ptr = err_table; ptr < end; ++ptr) {
        if (ptr->err_code == action) {
            code = ptr->err_code;
            desc = ptr->err_desc;
            should_exit = ptr->exit_on_err;
            break;
        }
    }

    STDOUT_LOGGER_ERROR(desc);

    if (should_exit) {
        goto err_exit;
    }
    return;

err_exit:
    if (code == ERR_IO_READ || code == ERR_IO_WRITE) {
        if (fp != NULL && *fp != NULL)
            fclose(*fp);
    } else if (code == ERR_IO_EXECUTE) {
        if (fp != NULL && *fp != NULL)
            pclose(*fp);
    }

    if (db != NULL && *db != NULL)
        sqlite3_close(*db);

    exit(code);
}
