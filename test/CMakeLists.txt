include(FetchContent)

FetchContent_Declare(
  unity
  GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
  GIT_TAG        v2.6.1
)
FetchContent_MakeAvailable(unity)

find_library(SQLITE sqlite3)

add_executable(database_test
  database_test.c
  ../src/database.c
  ../src/utils/log.c
)

target_link_libraries(database_test PRIVATE unity m ${SQLITE})
target_include_directories(database_test PRIVATE ../src/include ../src/utils/include)

add_test(NAME DatabaseTest COMMAND database_test)
