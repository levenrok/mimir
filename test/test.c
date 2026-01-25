#include "unity.h"

#include "fs/dir.h"
#include "fs/file.h"
#include "fs/temp.h"

void setUp(void) {}

void tearDown(void) {}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_temp_file_create);
    RUN_TEST(test_temp_file_delete);

    RUN_TEST(test_file_check_not_exist);
    RUN_TEST(test_file_check_exist);
    RUN_TEST(test_file_create_not_exist);

    RUN_TEST(test_dir_check_not_exist);
    RUN_TEST(test_dir_check_exist);

    return UNITY_END();
}
