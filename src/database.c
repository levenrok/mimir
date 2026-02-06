#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "include/database.h"

#include "utils/include/log.h"

#define LOG_DB_ERR(db) logger(ERROR, "db", "%s", sqlite3_errmsg(db))

static const int CONTENT_SIZE = 1024;
static const int SCRIPT_SIZE = (CONTENT_SIZE + 32);

static inline void cleanup_sqlite(sqlite3** db) {
    if (*db)
        sqlite3_close(*db);
}

static inline void cleanup_stmt(sqlite3_stmt** stmt) {
    if (*stmt)
        sqlite3_finalize(*stmt);
}

static inline void cleanup_errmsg(char** zErrMsg) {
    if (*zErrMsg)
        sqlite3_free(*zErrMsg);
}

DbErr openDatabase(Database* db, const char* path) {
    int rc = 0;

    db->db = NULL;
    db->path = path;

    rc = sqlite3_open_v2(path, &(db->db), SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    if (rc != SQLITE_OK) {
        LOG_DB_ERR(db->db);
        return DB_ERR_OPEN;
    }

    sqlite3_extended_result_codes(db->db, 1);

    return DB_OK;
}

DbErr initDatabase(Database* db) {
#if defined(__GNUC__) || defined(__clang__)
    sqlite3_stmt* stmt __attribute__((cleanup(cleanup_stmt)));
    char* zErrMsg __attribute__((cleanup(cleanup_errmsg))) = NULL;
#else
    sqlite3_stmt* stmt;
    char* zDbErrMsg = NULL;
#endif
    char* zSql =
        "CREATE TABLE IF NOT EXISTS scripts ("
        "id INTEGER PRIMARY KEY,"
        "name TEXT NOT NULL UNIQUE,"
        "shebang TEXT,"
        "content TEXT NOT NULL);";

    DbErr ret = DB_OK;

    int rc = sqlite3_exec(db->db, "PRAGMA journal_mode = WAL;", NULL, NULL, &zErrMsg);
    if (rc != SQLITE_OK) {
        logger(ERROR, "db", "%s", zErrMsg);
        ret = DB_ERR_INIT;
        goto err;
    }

    rc = sqlite3_exec(db->db, zSql, NULL, NULL, &zErrMsg);
    if (rc != SQLITE_OK) {
        LOG_DB_ERR(db->db);
        ret = DB_ERR_CREATE_TABLE;
        goto err;
    }

#if !defined(__GNUC__) || !defined(__clang__)
    cleanup_errmsg(&zDbErrMsg);
    cleanup_stmt(&stmt);
#endif
    return ret;

err:
#if !defined(__GNUC__) || !defined(__clang__)
    cleanup_errmsg(&zDbErrMsg);
    cleanup_stmt(&stmt);
#endif
    return ret;
}

bool pingDatabase(const char* path) {
#if defined(__GNUC__) || defined(__clang__)
    sqlite3* db __attribute__((cleanup(cleanup_sqlite)));
    sqlite3_stmt* stmt __attribute__((cleanup(cleanup_stmt)));
#else
    sqlite3* db;
    sqlite3_stmt* stmt;
#endif

    if (sqlite3_open_v2(path, &db, SQLITE_OPEN_READONLY, NULL) != SQLITE_OK)
        goto err;
    if (sqlite3_prepare_v2(db, "SELECT 1;", -1, &stmt, NULL) != SQLITE_OK)
        goto err;
    if (sqlite3_step(stmt) != SQLITE_ROW)
        goto err;

#if !defined(__GNUC__) || !defined(__clang__)
    cleanup_stmt(&stmt);
    cleanup_sqlite(&db);
#endif
    return true;

err:
#if !defined(__GNUC__) || !defined(__clang__)
    cleanup_stmt(&stmt);
    cleanup_sqlite(&db);
#endif
    return false;
}

DbErr insertScript(Database* db, char* name, char* content, char* shebang) {
#if defined(__GNUC__) || defined(__clang__)
    sqlite3_stmt* stmt __attribute__((cleanup(cleanup_stmt)));
#else
    sqlite3_stmt* stmt;
#endif
    const char* zSql = "INSERT INTO scripts (name, shebang, content) VALUES (?, ?, ?);";

    DbErr ret = DB_OK;

    int rc = sqlite3_prepare_v2(db->db, zSql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        ret = DB_ERR;
        goto err;
    }

    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, shebang, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, content, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        switch (rc) {
            case SQLITE_CONSTRAINT_UNIQUE:
                STDOUT_LOGGER_ERROR("script named '%s' already exists!", name);
                ret = DB_ERR_INSERT_UNIQUE;
                goto err;
            default:
                ret = DB_ERR_INSERT;
                goto err;
        }
    }

    STDOUT_LOGGER_SUCCESS("saved script '%s' to the database", name);

#if !defined(__GNUC__) || !defined(__clang__)
    cleanup_stmt(&stmt);
#endif
    return ret;

err:
    LOG_DB_ERR(db->db);
#if !defined(__GNUC__) || !defined(__clang__)
    cleanup_stmt(&stmt);
#endif
    return ret;
}

DbErr getScripts(Database* db) {
#if defined(__GNUC__) || defined(__clang__)
    sqlite3_stmt* stmt __attribute__((cleanup(cleanup_stmt)));
#else
    sqlite3_stmt* stmt;
#endif
    const char* zSql = "SELECT name, shebang, content FROM scripts;";

    DbErr ret = DB_OK;

    int rc = sqlite3_prepare_v2(db->db, zSql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        ret = DB_ERR;
        goto err;
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        const char* name = (const char*)sqlite3_column_text(stmt, 0);
        const char* shebang = (const char*)sqlite3_column_text(stmt, 1);
        const char* content = (const char*)sqlite3_column_text(stmt, 2);

        printf("---\n");
        printf("\033[1mname:\033[0m %s\n", name);
        printf("\033[1mshebang:\033[0m %s\n", shebang);
        printf("\033[1mcontent:\033[0m |\n%s", content);
    }

    if (rc != SQLITE_DONE) {
        ret = DB_ERR_SELECT;
        goto err;
    }

#if !defined(__GNUC__) || !defined(__clang__)
    sqlite3_finalize(stmt);
#endif
    return ret;

err:
    STDOUT_LOGGER_ERROR("%s", "cannot get scripts from the database!");
    LOG_DB_ERR(db->db);
#if !defined(__GNUC__) || !defined(__clang__)
    sqlite3_finalize(stmt);
#endif
    return ret;
}

DbErr getScriptContent(Database* db, char* name, char* buffer, bool get_shebang) {
    sqlite3_stmt* stmt;
    const char* zSql = get_shebang ? "SELECT shebang, content FROM scripts WHERE name = ?;"
                                   : "SELECT shebang, content FROM scripts WHERE name = ?;";

    DbErr ret = DB_OK;

    int rc = sqlite3_prepare_v2(db->db, zSql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        ret = DB_ERR;
        goto err;
    }

    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW)
        switch (rc) {
            case SQLITE_DONE:
                STDOUT_LOGGER_ERROR("no script named '%s' found in the database!", name);
                ret = DB_ERR_SELECT_NO_MATCH;
                goto err;
            default:
                STDOUT_LOGGER_ERROR("cannot get contents of '%s' from the database!", name);
                ret = DB_ERR_SELECT;
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
    LOG_DB_ERR(db->db);
    sqlite3_finalize(stmt);
    return ret;
}

DbErr deleteScript(Database* db, char* name) {
#if defined(__GNUC__) || defined(__clang__)
    sqlite3_stmt* stmt __attribute__((cleanup(cleanup_stmt)));
#else
    sqlite3_stmt* stmt;
#endif
    const char* zSql = "DELETE FROM scripts WHERE scripts.name = ?;";

    int rc = 0;

    DbErr ret = DB_OK;

    rc = sqlite3_prepare_v2(db->db, zSql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        ret = DB_ERR;
        LOG_DB_ERR(db->db);
        goto err;
    }

    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE) {
        int changes = sqlite3_changes(db->db);

        if (changes <= 0) {
            STDOUT_LOGGER_ERROR("cannot find script '%s' to delete", name);

            ret = DB_ERR_DELETE_NO_MATCH;
            goto err;
        }
    } else {
        ret = DB_ERR_DELETE;
        LOG_DB_ERR(db->db);
        goto err;
    }

    STDOUT_LOGGER_INFO("script %s deleted successfully", name);

#if !defined(__GNUC__) || !defined(__clang__)
    sqlite3_finalize(stmt);
#endif
    return ret;

err:
    LOG_DB_ERR(db->db);
#if !defined(__GNUC__) || !defined(__clang__)
    sqlite3_finalize(stmt);
#endif
    return ret;
}

DbErr closeDatabase(Database* db) {
    int rc = sqlite3_close_v2(db->db);

    if (rc != SQLITE_OK) {
        LOG_DB_ERR(db->db);
        db->db = NULL;
        return DB_ERR_CLOSE;
    }

    db->db = NULL;
    return DB_OK;
}
