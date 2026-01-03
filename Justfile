cc := 'clang'
cmake := 'cmake'

cmake_flags := '-DCMAKE_BUILD_TYPE=Debug'

cc_path := shell('which $1', cc)

srcs := shell('fd -e $1 -E $2 -E $3 | xargs', 'c', build_path, external_path)
include := shell('fd -e $1 -E $2 -E $3 | xargs', 'h', build_path, external_path)

build_path := 'build'
external_path := 'external'

# show available commands
default:
    @just -f {{justfile()}} --list

# build the executable
build: cmake
    @{{cmake}} --build {{build_path}}

# create cmake build system
cmake:
    @cmake -DCMAKE_C_COMPILER={{cc_path}} {{cmake_flags}} -S . -B {{build_path}}

# format source and header files
format:
    clang-format -i {{srcs}}
    clang-format -i {{include}}

# check for formatting in source and header files
format_check:
    clang-format --dry-run -Werror {{srcs}}
    clang-format --dry-run -Werror {{include}}

# clean the build directory
clean:
    rm -rf {{build_path}}
