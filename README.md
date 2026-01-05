<h1 align=center><code>mimir</code></h1>
<div align=center>
    <img src=".github/assets/icon.png" alt="icon" width="360px" />
</div>
<p align=center>
    <em>"the Rememberer" of all your scripts</em>
</p>
<br>

`mimir` is a database-driven solution to store and access your all your shell/python scripts and one liners in a single place.

## Usage

Just running `mimir` lists out the options available.

```console
$ mimir
Usage:
    ./mimir [name]
    ./mimir [options] [name]
Options:
    -c, --create <name>    Create a new script
    -b, --shebang <name>   Specify a shebang to the new script
    -l, --list             List all scripts
    -s, --show             Show contents of the specified script

```

### Create

You can create scripts or one liners using the `-c` or `--create` flag.

```sh
mimir -c greet
```

This will open a temporary file with the `$EDITOR` or `nano` if it's not set.


Optionally you can provide a `shebang` with `-b` or `--shebang`.

```sh
# use '' to specify the shebang.
mimir -c greet -b '#!/usr/bin/env bash'
```

### List

All the scripts stored can be viewed with the `-l` or `--list`.

```console
$ mimir -l
---
name: greet
shebang: #!/usr/bin/env bash
content: |
echo "Hello, from mimir!"

```

To view the content of a specific script, use the `-s` or `--show` flag with the name of the script.

```console
$ mimir -s greet
#!/usr/bin/env bash

echo "Hello, from mimir!"

```
<br>

> [!NOTE]
> If the `shebang` is specified, It will prepend the `shebang` to the top of the contents.

### Run

Any script (or one liner) can be ran will just calling `mimir` and the name of the script.

``` console
$ mimir greet
Hello, from mimir!
```

## Installation

### Prerequisites

- Clang 16.0+ or GCC 14.0+ (Tested on Clang 21.1.2)
- CMake >= 3.14

Optional:

- [`just`](https://github.com/casey/just)
- [`fd`](https://github.com/sharkdp/fd)

### Build

1. Setup CMake

```sh
cmake -DCMAKE_C_COMPILER=$(which clang) -S . -B build
```

2. Build the Executable

```sh
cmake --build build
```

Or use the `justfile`:

```sh
just cmake
just build
```

3. Give it a spin

```console
$ cd build
$ ./mimir
Usage:
    ./mimir [name]
    ./mimir [options] [name]
Options:
    -c, --create <name>    Create a new script
    -b, --shebang <name>   Specify a shebang to the new script
    -l, --list             List all scripts
    -s, --show             Show contents of the specified script

```

### Install

1. Set the build type of CMake

```sh
cmake -DCMAKE_C_COMPILER=$(which clang) -DCMAKE_BUILD_TYPE=Release -S . -B build
```

2. Build the Executable

```sh
cmake --build build
```

3. Run the install command

```sh
cmake --install build
```

Or use the `justfile`:

```sh
just cmake Release # set the build type to 'Release'
just install
```
<br>

> [!TIP]
> If you want to install the executable to a custom directory in the `$PATH`, you can use the `--prefix` flag with the installation command.
> ```console
> $ cmake --install . --prefix ~/.local # will be installed in '~/.local/bin'
> -- Install configuration: "Release"
> -- Installing: /home/levenrok/.local/bin/mimir
> $ which mimir
> /home/levenrok/.local/bin/mimir
> ```
