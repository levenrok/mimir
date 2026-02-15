#ifndef MIMIR_DATABASE_H
#define MIMIR_DATABASE_H

#include <sqlite3.h>
#include <stdbool.h>

typedef struct Database {
    sqlite3* db;
    const char* path;
} Database;

typedef enum DbErr {
    DB_OK,
    DB_ERR,
    DB_ERR_OPEN,
    DB_ERR_INIT,
    DB_ERR_CREATE_TABLE,
    DB_ERR_INSERT,
    DB_ERR_INSERT_UNIQUE,
    DB_ERR_SELECT,
    DB_ERR_SELECT_NO_MATCH,
    DB_ERR_DELETE,
    DB_ERR_DELETE_NO_MATCH,
    DB_ERR_CLOSE,
} DbErr;

DbErr openDatabase(Database* db, const char* path, const char* log_path);
DbErr initDatabase(Database* db, const char* log_path);
DbErr insertScript(Database* db, char* name, char* content, char* shebang);
DbErr getScripts(Database* db);
DbErr getScriptContent(Database* db, char* name, char* buffer, bool get_shebang);
DbErr deleteScript(Database* db, char* name);
DbErr closeDatabase(Database* db);

#endif  // MIMIR_DATABASE_H
