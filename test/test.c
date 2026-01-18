#include "unity.h"

#include "fs/temp.h"

void setUp(void) {}

void tearDown(void) {}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_temp_file_create);
    RUN_TEST(test_temp_file_delete);

    return UNITY_END();
}
