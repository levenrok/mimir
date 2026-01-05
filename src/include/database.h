#ifndef __MIMIR_DATABASE_H__
#define __MIMIR_DATABASE_H__

#include <sqlite3.h>
#include <stdbool.h>

#include "err.h"

typedef struct {
    char name[128];
    char contents[1024];
    char shebang[32];
} ScriptInfo;

mimir_error openDatabase(sqlite3** db);
mimir_error insertScript(sqlite3* db, char* name, char* content, char* shebang);
mimir_error getScripts(sqlite3* db);
mimir_error getScript(sqlite3* db, char* name, char* buffer, bool shebang);
void closeDatabase(sqlite3** db);

#endif  // __MIMIR_DATABASE_H__
