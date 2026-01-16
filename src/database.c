#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "include/database.h"

#include "sqlite3.h"
#include "utils/include/fs.h"
#include "utils/include/log.h"

#define LOG_DB_ERR(db) logger(ERROR, "db", "%s", sqlite3_errmsg(db));

static const int CONTENT_SIZE = 1024;
static const int SCRIPT_SIZE = (CONTENT_SIZE + 32);

Err openDatabase(sqlite3** db) {
    sqlite3_stmt* stmt;

    char database_path[256];

    int rc = 0;
    char* zErrMsg = NULL;

    Err ret = OK;

    if (getAppDataPath(database_path, "db.sqlite3") != OK) {
        STDOUT_LOGGER_ERROR("%s", "cannot access the database");

        ret = ERR_DB_ACCESS;
        goto err;
    }

    rc = sqlite3_open_v2(database_path, db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    if (rc != SQLITE_OK) {
        STDOUT_LOGGER_ERROR("%s", "cannot open the database");

        ret = ERR_DB_OPEN;
        goto err;
    }

    rc = sqlite3_exec(*db, "PRAGMA journal_mode = wal;", NULL, NULL, &zErrMsg);
    if (rc != SQLITE_OK) {
        logger(WARNING, "db", "%s", zErrMsg);
    }

    const char* sql =
        "CREATE TABLE IF NOT EXISTS scripts ("
        "id INTEGER PRIMARY KEY,"
        "name TEXT NOT NULL UNIQUE,"
        "shebang TEXT,"
        "content TEXT NOT NULL);";

    rc = sqlite3_prepare_v2(*db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        STDOUT_LOGGER_ERROR("%s", "cannot create the database table");

        sqlite3_finalize(stmt);
        ret = ERR_DB_CREATE_TABLE;
        goto err;
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        STDOUT_LOGGER_ERROR("%s", "cannot create the database table");

        sqlite3_finalize(stmt);
        ret = ERR_DB_CREATE_TABLE;
        goto err;
    }

    sqlite3_extended_result_codes(*db, 1);

    sqlite3_finalize(stmt);
    sqlite3_free(zErrMsg);
    return ret;

err:
    LOG_DB_ERR(*db);
    sqlite3_free(zErrMsg);
    return ret;
}

Err insertScript(sqlite3* db, char* name, char* content, char* shebang) {
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO scripts (name, shebang, content) VALUES (?, ?, ?);";

    Err ret = OK;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
        goto err;

    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, shebang, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, content, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        switch (rc) {
            case SQLITE_CONSTRAINT_UNIQUE:
                STDOUT_LOGGER_ERROR("script named '%s' already exists!", name);
                ret = ERR_DB_INSERT_UNIQUE;
                goto err;
            default:
                ret = ERR_DB_INSERT;
                goto err;
        }
    }

    STDOUT_LOGGER_SUCCESS("saved script '%s' to the database", name);

    sqlite3_finalize(stmt);
    return ret;

err:
    LOG_DB_ERR(db);
    sqlite3_finalize(stmt);
    return ret;
}

Err getScripts(sqlite3* db) {
    sqlite3_stmt* stmt;
    const char* sql = "SELECT name, shebang, content FROM scripts;";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
        goto err;

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        const char* name = (const char*)sqlite3_column_text(stmt, 0);
        const char* shebang = (const char*)sqlite3_column_text(stmt, 1);
        const char* content = (const char*)sqlite3_column_text(stmt, 2);

        printf("---\n");
        printf("\033[1mname:\033[0m %s\n", name);
        printf("\033[1mshebang:\033[0m %s\n", shebang);
        printf("\033[1mcontent:\033[0m |\n%s", content);
    }

    if (rc != SQLITE_DONE)
        goto err;

    sqlite3_finalize(stmt);
    return OK;

err:
    STDOUT_LOGGER_ERROR("%s", "cannot get scripts from the database!");
    LOG_DB_ERR(db);
    sqlite3_finalize(stmt);
    return ERR_DB_SELECT;
}

Err getScriptContent(sqlite3* db, char* name, char* buffer, bool get_shebang) {
    sqlite3_stmt* stmt;

    Err ret = OK;

    const char* sql = get_shebang ? "SELECT shebang, content FROM scripts WHERE name = ?;"
                                  : "SELECT shebang, content FROM scripts WHERE name = ?;";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        ret = ERR_DB_SELECT;
        goto err;
    }

    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW)
        switch (rc) {
            case SQLITE_DONE:
                STDOUT_LOGGER_ERROR("no script named '%s' found in the database!", name);
                ret = ERR_DB_SELECT_NO_MATCH;
                goto err;
            default:
                STDOUT_LOGGER_ERROR("cannot get contents of '%s' from the database!", name);
                ret = ERR_DB_SELECT;
                goto err;
        }

    const char* shebang = (const char*)sqlite3_column_text(stmt, 0);
    const char* content = (const char*)sqlite3_column_text(stmt, 1);

    shebang = shebang == NULL ? "" : shebang;
    content = content == NULL ? "" : content;

    if (buffer != NULL) {
        if (strlen(shebang) > 0) {
            snprintf(buffer, SCRIPT_SIZE, "%s\n\n%s", shebang, content);
        } else {
            snprintf(buffer, SCRIPT_SIZE, "%s", content);
        }
    } else {
        if (strlen(shebang) > 0) {
            printf("%s\n\n%s", shebang, content);
        } else {
            printf("%s", content);
        }
    }

    sqlite3_finalize(stmt);
    return ret;

err:
    LOG_DB_ERR(db);
    sqlite3_finalize(stmt);
    return ret;
}

void closeDatabase(sqlite3** db) {
    if (db != NULL && *db != NULL) {
        sqlite3_close_v2(*db);
        *db = NULL;
    }
}
