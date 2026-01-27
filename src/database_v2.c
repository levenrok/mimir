#include <stdio.h>

#include "include/database_v2.h"

Err openDatabaseV2(Database* db, const char* path) {
    int rc = 0;
    Err ret = OK;

    db->path = path;
    db->db = NULL;
    db->err_msg = NULL;

    rc = sqlite3_open_v2(path, &(db->db), SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    if (rc != SQLITE_OK) {
        db->err_msg = sqlite3_errmsg(db->db);

        ret = ERR_DB_OPEN;
        return ret;
    }

    sqlite3_extended_result_codes(db->db, 1);

    return ret;
}

Err closeDatabaseV2(Database* db) {
    int rc = sqlite3_close_v2(db->db);

    if (rc != SQLITE_OK) {
        db->err_msg = sqlite3_errmsg(db->db);
        db->db = NULL;
        return ERR_DB_CLOSE;
    }

    db->db = NULL;
    db->err_msg = NULL;
    return OK;
}
