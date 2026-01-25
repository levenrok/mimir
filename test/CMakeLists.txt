include(FetchContent)

FetchContent_Declare(
  unity
  GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
  GIT_TAG        v2.6.1
)
FetchContent_MakeAvailable(unity)

add_executable(mimir_test
    test.c
    fs/dir.c
    fs/file.c
    fs/temp.c
    ../src/utils/fs.c
    ../src/utils/log.c
)
target_link_libraries(mimir_test unity m)
target_include_directories(mimir_test PRIVATE ../src/include ../src/utils/include)

add_test(NAME MimirTest COMMAND mimir_test)
