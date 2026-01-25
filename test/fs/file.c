#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include "unity.h"

#include "../../src/utils/include/errs.h"
#include "../../src/utils/include/fs.h"
#include "file.h"

#define MIMIR_TEST_FILE_FILEPATH "/tmp/test_file"
#define MIMIR_TEST_FILE_FILEPATH_CREATE                  \
    do {                                                 \
        FILE* fp = fopen(MIMIR_TEST_FILE_FILEPATH, "w"); \
        fclose(fp);                                      \
    } while (0)
#define MIMIR_TEST_FILE_FILEPATH_REMOVE                  \
    do {                                                 \
        if (access(MIMIR_TEST_FILE_FILEPATH, F_OK) == 0) \
            remove(MIMIR_TEST_FILE_FILEPATH);            \
    } while (0)

void test_file_check_not_exist() {
    MIMIR_TEST_FILE_FILEPATH_REMOVE;

    Err rc = ensureFileExists(MIMIR_TEST_FILE_FILEPATH, false);

    TEST_ASSERT_EQUAL(ERR_FS_FILE_NOT_EXIST, rc);
}

void test_file_check_exist() {
    MIMIR_TEST_FILE_FILEPATH_REMOVE;

    MIMIR_TEST_FILE_FILEPATH_CREATE;

    Err rc = ensureFileExists(MIMIR_TEST_FILE_FILEPATH, false);

    TEST_ASSERT_EQUAL(OK, rc);

    MIMIR_TEST_FILE_FILEPATH_REMOVE;
}

void test_file_create_not_exist() {
    MIMIR_TEST_FILE_FILEPATH_REMOVE;

    Err rc = ensureFileExists(MIMIR_TEST_FILE_FILEPATH, true);

    TEST_ASSERT_EQUAL(OK_FS_FILE_CREATE, rc);

    MIMIR_TEST_FILE_FILEPATH_REMOVE;
}
