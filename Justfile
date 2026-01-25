cc := 'clang'
cmake := 'cmake'

srcs := shell('fd -e $1 -0 -E $2 -E $3 | xargs -0', 'c', build_path, external_path)
include := shell('fd -e $1 -0 -E $2 -E $3 | xargs -0', 'h', build_path, external_path)

src_path := 'src'
build_path := 'build'
external_path := 'external'

proc := shell('nproc')

# show available commands
default:
    @just -f {{justfile()}} --list

# install the executable
install: build
    @{{cmake}} --install {{build_path}}

# run the tests
test:
    @ctest -j{{proc}} --test-dir {{build_path}}

# build the executable
build:
    @{{cmake}} --build {{build_path}} --parallel {{proc}}

# create cmake build system
cmake compiler=cc type="Debug":
    @cmake -DCMAKE_C_COMPILER={{compiler}} -DCMAKE_BUILD_TYPE={{type}} -S . -B {{build_path}}

# format source and header files
format:
    clang-format -i {{srcs}}
    clang-format -i {{include}}

# check for formatting in source and header files
format_check:
    clang-format --dry-run -Werror {{srcs}}
    clang-format --dry-run -Werror {{include}}

tidy:
    @run-clang-tidy -p {{build_path}} -j{{proc}} "{{src_path}}.*"

# clean the build directory
clean:
    @{{cmake}} --build {{build_path}} --target clean

# remove the build directory
clean_all:
    rm -rf {{build_path}}
