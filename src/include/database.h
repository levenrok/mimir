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

Err openDatabase(sqlite3** db);
Err insertScript(sqlite3* db, char* name, char* content, char* shebang);
Err getScripts(sqlite3* db);
Err getScriptContent(sqlite3* db, char* name, char* buffer, bool get_shebang);
void closeDatabase(sqlite3** db);

#endif  // __MIMIR_DATABASE_H__
