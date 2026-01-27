#include <stdio.h>
#include <string.h>
#include "sqlite3.h"
#include "unity.h"

#include "../src/include/database_v2.h"
#include "../src/utils/include/errs.h"
#include "unity_internals.h"

Database db;

void setUp(void) {
    openDatabaseV2(&db, "test.sqlite3");
}

void tearDown(void) {
    remove(db.path);
}

void test_open_database() {
    Database open;
    sqlite3_stmt* stmt;
    const char* db_name = "open.sqlite3";

    TEST_ASSERT_EQUAL(OK, openDatabaseV2(&open, db_name));
    TEST_ASSERT_EQUAL(0, strncmp(db_name, open.path, strlen(db_name)));
    TEST_ASSERT_NOT_NULL(open.db);
    TEST_ASSERT_NULL(open.err_msg);

    TEST_ASSERT_EQUAL(SQLITE_OK, sqlite3_prepare_v2(open.db, "SELECT 1;", -1, &stmt, NULL));
    TEST_ASSERT_EQUAL(SQLITE_ROW, sqlite3_step(stmt));

    sqlite3_finalize(stmt);
    sqlite3_close_v2(open.db);
    remove(db_name);
}

void test_close_database() {
    Database close;
    sqlite3_stmt* stmt;
    const char* db_name = "close.sqlite3";

    close.path = db_name;

    TEST_ASSERT_EQUAL(SQLITE_OK,
                      sqlite3_open_v2(db_name, &(close.db), SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL));

    TEST_ASSERT_EQUAL(OK, closeDatabaseV2(&close));
    TEST_ASSERT_NULL(close.db);
    TEST_ASSERT_NULL(close.err_msg);

    TEST_ASSERT_NOT_EQUAL(SQLITE_OK, sqlite3_prepare_v2(close.db, "SELECT 1;", -1, &stmt, NULL));
    TEST_ASSERT_NOT_EQUAL(SQLITE_ROW, sqlite3_step(stmt));

    sqlite3_finalize(stmt);
    remove(db_name);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_open_database);
    RUN_TEST(test_close_database);

    return UNITY_END();
}
