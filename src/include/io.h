#ifndef __MIMIR_IO_H__
#define __MIMIR_IO_H__

#include <sqlite3.h>
#include <stdio.h>

#include "../utils/include/errs.h"

/**
 * Helper function to get the script's content
 */
Err importScriptContent(char* buffer, int buffer_size, FILE** fp);
Err runScriptContent(sqlite3* db, char* name, FILE** fp);

void printMan(char* program_name);
void printHelp(char* program_name);

#endif  // __MIMIR_IO_H__
