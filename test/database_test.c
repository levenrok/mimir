#include <stdio.h>
#include <string.h>
#include "sqlite3.h"
#include "unity.h"

#include "../src/include/database_v2.h"
#include "../src/utils/include/errs.h"

Database db;

void setUp(void) {
    db.path = "test.sqlite3";
    db.err_msg = NULL;

    int rc = sqlite3_open_v2(db.path, &(db.db), SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    if (rc != SQLITE_OK)
        TEST_ABORT();
}

void tearDown(void) {
    int rc = sqlite3_close_v2(db.db);
    if (rc != SQLITE_OK)
        TEST_ABORT();

    rc = remove(db.path);
    if (rc != 0)
        TEST_ABORT();
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

void test_init_database() {
    sqlite3_stmt* stmt;
    char* zSql =
        "CREATE TABLE IF NOT EXISTS scripts ("
        "id INTEGER PRIMARY KEY,"
        "name TEXT NOT NULL UNIQUE,"
        "shebang TEXT,"
        "content TEXT NOT NULL)";

    TEST_ASSERT_EQUAL(OK, initDatabaseV2(&db));
    TEST_ASSERT_NULL(db.err_msg);

    TEST_ASSERT_EQUAL(SQLITE_OK, sqlite3_prepare_v2(db.db, "PRAGMA journal_mode;", -1, &stmt, NULL));
    TEST_ASSERT_EQUAL(SQLITE_ROW, sqlite3_step(stmt));
    TEST_ASSERT_EQUAL(0, strncmp("wal", (const char*)sqlite3_column_text(stmt, 0), 3));

    sqlite3_reset(stmt);

    TEST_ASSERT_EQUAL(
        SQLITE_OK, sqlite3_prepare_v2(db.db, "SELECT sql FROM sqlite_master WHERE type='table' AND name='scripts';", -1,
                                      &stmt, NULL));
    TEST_ASSERT_EQUAL(SQLITE_ROW, sqlite3_step(stmt));

    sqlite3_finalize(stmt);
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
    RUN_TEST(test_init_database);
    RUN_TEST(test_close_database);

    return UNITY_END();
}
