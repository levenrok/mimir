cc := 'clang'
cmake := 'cmake'

cc_path := shell('which $1', cc)

srcs := shell('fd -e $1 -0 -E $2 -E $3 | xargs -0', 'c', build_path, external_path)
include := shell('fd -e $1 -0 -E $2 -E $3 | xargs -0', 'h', build_path, external_path)

build_path := 'build'
external_path := 'external'

# show available commands
default:
    @just -f {{justfile()}} --list

install: build
    @{{cmake}} --install {{build_path}}

# build the executable
build:
    @{{cmake}} --build {{build_path}}

# create cmake build system
cmake type="Debug":
    @cmake -DCMAKE_C_COMPILER={{cc_path}} -DCMAKE_BUILD_TYPE={{type}} -S . -B {{build_path}}

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
