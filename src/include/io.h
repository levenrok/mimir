#ifndef MIMIR_IO_H
#define MIMIR_IO_H

#include <sqlite3.h>
#include <stdio.h>

#include "database.h"

typedef enum IoErr {
    IO_OK,
    IO_ERR,
    IO_ERR_READ,
    IO_ERR_WRITE,
    IO_ERR_EXECUTE,
} IoErr;

/**
 * Helper function to get the script's content
 */
IoErr importScriptContent(char* buffer, int buffer_size, FILE** fp);
IoErr runScriptContent(Database* db, char* name, FILE** fp);

void printMan(char* program_name);
void printHelp(char* program_name);

#endif  // MIMIR_IO_H
