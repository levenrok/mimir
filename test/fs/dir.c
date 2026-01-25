#include <stdbool.h>
#include <sys/stat.h>
#include "unistd.h"
#include "unity.h"

#include "../../src/utils/include/errs.h"
#include "../../src/utils/include/fs.h"
#include "dir.h"

#define MIMIR_TEST_DIR_DIRPATH "/tmp/test_dir/"
#define MIMIR_TEST_DIR_DIRPATH_CREATE        \
    do {                                     \
        mkdir(MIMIR_TEST_DIR_DIRPATH, 0700); \
    } while (0)
#define MIMIR_TEST_DIR_DIRPATH_REMOVE               \
    do {                                            \
        struct stat st = {0};                       \
        if (stat(MIMIR_TEST_DIR_DIRPATH, &st) == 0) \
            rmdir(MIMIR_TEST_DIR_DIRPATH);          \
    } while (0)

void test_dir_check_not_exist() {
    MIMIR_TEST_DIR_DIRPATH_REMOVE;

    Err rc = ensureDirectoryExists(MIMIR_TEST_DIR_DIRPATH, false);

    TEST_ASSERT_EQUAL(ERR_FS_DIR_NOT_EXIST, rc);

    MIMIR_TEST_DIR_DIRPATH_REMOVE;
}

void test_dir_check_exist() {
    MIMIR_TEST_DIR_DIRPATH_REMOVE;

    MIMIR_TEST_DIR_DIRPATH_CREATE;

    Err rc = ensureDirectoryExists(MIMIR_TEST_DIR_DIRPATH, false);

    TEST_ASSERT_EQUAL(OK, rc);

    MIMIR_TEST_DIR_DIRPATH_REMOVE;
}

void test_dir_create_not_exist() {}
