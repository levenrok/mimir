#include <stdio.h>

#include "include/database_v2.h"

Err openDatabaseV2(Database* db, const char* path) {
    int rc = 0;

    db->path = path;
    db->db = NULL;
    db->err_msg = NULL;

    rc = sqlite3_open_v2(path, &(db->db), SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    if (rc != SQLITE_OK) {
        db->err_msg = sqlite3_errmsg(db->db);
        return ERR_DB_OPEN;
    }

    sqlite3_extended_result_codes(db->db, 1);

    return OK;
}

Err initDatabaseV2(Database* db) {
    sqlite3_stmt* stmt;
    char* zSql =
        "CREATE TABLE IF NOT EXISTS scripts ("
        "id INTEGER PRIMARY KEY,"
        "name TEXT NOT NULL UNIQUE,"
        "shebang TEXT,"
        "content TEXT NOT NULL);";
    char* zErrMsg = NULL;

    db->err_msg = NULL;

    int rc = sqlite3_exec(db->db, "PRAGMA journal_mode = WAL;", NULL, NULL, &zErrMsg);
    if (rc != SQLITE_OK) {
        goto err;
    }

    rc = sqlite3_prepare_v2(db->db, zSql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        goto err;
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        goto err;
    }

    sqlite3_free(zErrMsg);
    sqlite3_finalize(stmt);
    return OK;

err:
    db->err_msg = sqlite3_errmsg(db->db);
    sqlite3_free(zErrMsg);
    return ERR_DB_INIT;
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
