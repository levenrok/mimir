#ifndef __MIMIR_IO_H__
#define __MIMIR_IO_H__

#include <sqlite3.h>
#include <stdio.h>

#include "../utils/include/errs.h"

/**
 * Helper function to get the script's content
 */
err_t importScriptContent(char* buffer, int buffer_size, FILE** fp);
err_t runScriptContent(sqlite3* db, char* name, FILE** fp);

void printHelp(char* program_name);

#endif  // __MIMIR_IO_H__
