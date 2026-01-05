#ifndef __MIMIR_IO_H__
#define __MIMIR_IO_H__

#include <sqlite3.h>

#include "err.h"

/**
 * Helper function to get the script's content
 */
mimir_error importScriptContent(char* buffer, int buffer_size);
mimir_error runScriptContent(sqlite3* db, char* name);

void printHelp(char* program_name);

#endif  // __MIMIR_IO_H__
