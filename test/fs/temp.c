#include <unistd.h>
#include "unity.h"

#include "../../src/utils/include/fs.h"
#include "temp.h"

void test_temp_file_create(void) {
    char* file = createTempFile();

    bool file_exists = access(file, F_OK) == 0 ? true : false;

    TEST_ASSERT_TRUE(file_exists);
    TEST_ASSERT_NOT_NULL(file);

    deleteTempFile(&file);
}

void test_temp_file_delete(void) {
    char* file = createTempFile();
    deleteTempFile(&file);

    bool file_exists = access(file, F_OK) == 0 ? true : false;

    TEST_ASSERT_FALSE(file_exists);
    TEST_ASSERT_NULL(file);
}
