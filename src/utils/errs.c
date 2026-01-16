#include <stdbool.h>
#include <stdlib.h>

#include "include/errs.h"
#include "include/log.h"

#undef X_ERR
#define X_ERR(code, tag, desc, exit) {code, tag, desc, exit},

typedef struct ErrInfo {
    Err err_code;
    char* err_tag;
    char* err_desc;
    bool exit_on_err;
} ErrInfo;

static ErrInfo err_table[] = {ERR_INFO};

void handle_err(Err action, sqlite3** db, FILE** fp) {
    if (action == OK)
        return;

    Err code = ERR;
    char* tag = NULL;
    char* desc = NULL;
    bool should_exit = true;

    ErrInfo* end = err_table + ERR_INFO_COUNT;
    for (ErrInfo* ptr = err_table; ptr < end; ++ptr) {
        if (ptr->err_code == action) {
            code = ptr->err_code;
            tag = ptr->err_tag;
            desc = ptr->err_desc;
            should_exit = ptr->exit_on_err;
            break;
        }
    }

    logger(ERROR, tag, desc);

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
