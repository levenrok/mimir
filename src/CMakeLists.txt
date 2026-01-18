include(GNUInstallDirs)

find_library(SQLITE sqlite3)

set(SRCS
    ../main.c
    database.c
    io.c
    utils/errs.c
    utils/fs.c
    utils/log.c
    ../external/argtable3.c
    ../external/cJSON.c
    ../external/ini.c
)

set(EXE mimir)

add_compile_options(-Wall -Werror -Wpedantic)
include_directories(src/include src/utils/include external)

add_executable(${EXE} ${SRCS})
target_link_libraries(${EXE} PRIVATE m ${SQLITE})

install(FILES docs/mimir.1 DESTINATION share/man/man1)

install(TARGETS ${EXE}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
