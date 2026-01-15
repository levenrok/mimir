#ifndef __MIMIR_DATABASE_H__
#define __MIMIR_DATABASE_H__

#include <sqlite3.h>
#include <stdbool.h>

#include "../utils/include/errs.h"

typedef struct {
    char name[128];
    char contents[1024];
    char shebang[32];
} ScriptInfo;

err_t openDatabase(sqlite3** db);
err_t insertScript(sqlite3* db, char* name, char* content, char* shebang);
err_t getScripts(sqlite3* db);
err_t getScript(sqlite3* db, char* name, char* buffer, bool shebang);
void closeDatabase(sqlite3** db);

#endif  // __MIMIR_DATABASE_H__
