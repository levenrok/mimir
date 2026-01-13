cmake_minimum_required(VERSION 3.14)
project(mimir C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(GNUInstallDirs)

find_library(SQLITE sqlite3)

set(SRCS
    main.c 
    src/database.c
    src/io.c
    src/utils/err.c
    src/utils/fs.c
    src/utils/log.c
    external/cJSON.c
)

set(EXE mimir)

add_compile_options(-Wall -Werror -Wpedantic)
include_directories(src/include src/utils/include external)

add_executable(${EXE} ${SRCS})
target_link_libraries(${EXE} PRIVATE ${SQLITE})

install(FILES docs/mimir.1 DESTINATION share/man/man1)

install(TARGETS ${EXE}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
