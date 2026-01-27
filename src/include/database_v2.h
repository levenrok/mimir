#ifndef MIMIR_DATABASE_H
#define MIMIR_DATABASE_H

#include <sqlite3.h>

#include "../utils/include/errs.h"

typedef struct Database {
    const char* path;
    sqlite3* db;
    const char* err_msg;
} Database;

Err openDatabaseV2(Database* db, const char* path);
Err initDatabaseV2(Database* db);
Err closeDatabaseV2(Database* db);

#endif  // MIMIR_DATABASE_H
