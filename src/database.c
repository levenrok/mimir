#include "database.h"

static const int CONTENT_SIZE = 1024;
static const int SCRIPT_SIZE = (CONTENT_SIZE + 32);

static int callbackInsertDatabase(void* _data, int argc, char** argv, char** col_name);
static int callbackSelectAllDatabase(void* _data, int argc, char** argv, char** azColName);
static int callbackSelectScriptDatabase(void* data, int argc, char** argv, char** azColName);
static int callbackSelectContentDatabase(void* data, int argc, char** argv, char** azColName);

mimir_error openDatabase(sqlite3** db) {
    int err = 0;
    char* err_msg = NULL;

    char database_path[256];

    (void)getAppDataPath(database_path, "db.sqlite3");

    err = sqlite3_open_v2(database_path, db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    if (err != SQLITE_OK) {
        logStatus(ERROR, "db", true, "%s :(", sqlite3_errmsg(*db));
        goto err;
    }

    err = sqlite3_exec(*db, "PRAGMA journal_mode = wal;", NULL, NULL, &err_msg);
    if (err != SQLITE_OK) {
        logStatus(ERROR, "db", false, "%s :(", err_msg);
        goto err;
    }

    err = sqlite3_exec(*db,
                       "CREATE TABLE IF NOT EXISTS scripts ("
                       "id INTEGER PRIMARY KEY,"
                       "name TEXT NOT NULL UNIQUE,"
                       "shebang TEXT,"
                       "content TEXT NOT NULL);",
                       callbackInsertDatabase, NULL, &err_msg);
    if (err != SQLITE_OK) {
        logStatus(ERROR, "db", false, "%s :(", err_msg);
        goto err;
    }

    sqlite3_extended_result_codes(*db, 1);

    logStatus(SUCCESS, "io", false, "database opened sucessfully! :)");

    sqlite3_free(err_msg);
    return OK;

err:
    sqlite3_free(err_msg);
    return DB_OPEN_ERROR;
}

mimir_error insertScript(sqlite3* db, char* name, char* content, char* shebang) {
    int err = 0;
    char* err_msg = NULL;

    char* query = sqlite3_mprintf("INSERT INTO scripts (name, shebang, content) VALUES ('%q', '%q', %Q);", name,
                                  shebang, content);

    err = sqlite3_exec(db, query, callbackInsertDatabase, NULL, &err_msg);

    if (err != SQLITE_OK) {
        if (err == SQLITE_CONSTRAINT_UNIQUE) {
            logStatus(ERROR, "db", false, "%s :(", err_msg);
            printf("\033[31mscript name must be unique :\\\033[0m");
        } else {
            logStatus(ERROR, "db", true, "%s :(", err_msg);
        }
        goto err;
    }

    logStatus(SUCCESS, "io", true, "script saved sucessfully! :)");

    sqlite3_free(query);
    sqlite3_free(err_msg);
    return OK;

err:
    sqlite3_free(query);
    sqlite3_free(err_msg);
    return DB_INSERT_ERROR;
}

mimir_error getScripts(sqlite3* db) {
    int err = 0;
    char* err_msg = NULL;

    err = sqlite3_exec(db, "SELECT name, shebang, content FROM scripts;", callbackSelectAllDatabase, NULL, &err_msg);
    if (err != SQLITE_OK) {
        logStatus(ERROR, "db", true, "%s :(", err_msg);
        goto err;
    }

    sqlite3_free(err_msg);
    return OK;

err:
    sqlite3_free(err_msg);
    return DB_SELECT_ERROR;
}

mimir_error getScript(sqlite3* db, char* name, char* buffer, bool shebang) {
    char* query;

    int err = 0;
    char* err_msg = NULL;

    if (!shebang) {
        query = sqlite3_mprintf("SELECT content FROM scripts WHERE name = '%q'", name);

        err = sqlite3_exec(db, query, callbackSelectContentDatabase, buffer, &err_msg);
    } else {
        query = sqlite3_mprintf("SELECT shebang, content FROM scripts WHERE name = '%q'", name);

        err = sqlite3_exec(db, query, callbackSelectScriptDatabase, buffer, &err_msg);
    }

    if (err != SQLITE_OK) {
        logStatus(ERROR, "db", true, "%s :(", err_msg);
        goto err;
    }

    sqlite3_free(query);
    sqlite3_free(err_msg);
    return OK;

err:
    sqlite3_free(query);
    sqlite3_free(err_msg);
    return DB_SELECT_ERROR;
}

void closeDatabase(sqlite3** db) {
    if (db != NULL && *db != NULL) {
        sqlite3_close_v2(*db);
        *db = NULL;
        logStatus(SUCCESS, "io", false, "database closed sucessfully! :)");
    }
}

static int callbackInsertDatabase(void* _data, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }

    return 0;
}

int callbackSelectAllDatabase(void* _data, int argc, char** argv, char** azColName) {
    printf("---\n");
    for (int j = 0; j < argc; j++) {
        if (strcmp(azColName[j], "content") == 0) {
            printf("\033[1m%s:\033[0m |\n%s", azColName[j], argv[j]);
        } else {
            printf("\033[1m%s:\033[0m %s\n", azColName[j], argv[j]);
        }
    }

    return 0;
}

int callbackSelectScriptDatabase(void* data, int argc, char** argv, char** azColName) {
    char* shebang = NULL;
    char* content = NULL;

    for (int j = 0; j < argc; j++) {
        if (strcmp(azColName[j], "shebang") == 0) {
            shebang = argv[j];
        } else {
            content = argv[j];
        }
    }

    if (shebang == NULL && content == NULL) {
        return 1;
    }

    data == NULL ? printf("%s\n\n%s", shebang, content) : snprintf(data, SCRIPT_SIZE, "%s\n\n%s", shebang, content);

    return 0;
}

int callbackSelectContentDatabase(void* data, int argc, char** argv, char** azColName) {
    char* content;

    if (data == NULL) {
        return 1;
    } else {
        for (int j = 0; j < argc; j++) {
            if (strcmp(azColName[j], "content") == 0) {
                content = argv[j];
                snprintf(data, CONTENT_SIZE, "%s", content);
            }
        }
    }

    return 0;
}
