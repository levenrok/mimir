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
    -l, --list    List all scripts
    -s, --show    Show contents of the specified script

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
