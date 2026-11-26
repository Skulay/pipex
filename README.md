*This project has been created as part of the 42 curriculum by alehamad.*

# pipex

A program that reproduces the shell pipe `< infile cmd1 | cmd2 > outfile`, written from scratch in C.

![language](https://img.shields.io/badge/language-C-000000?style=flat-square)
![school](https://img.shields.io/badge/school-42-000000?style=flat-square)

## Overview

**pipex** takes an input file, two commands and an output file, and behaves exactly like this shell line:

```bash
< infile cmd1 | cmd2 > outfile
```

The goal of the project is to understand what a shell does behind a `|`:

- how a **pipe** connects the output of one process to the input of another
- how processes are created with **`fork`** and replaced with **`execve`**
- how file descriptors are redirected with **`dup2`**, and why every unused end must be **closed**
- how a command is found in the **`PATH`** environment variable
- how the **exit status** of a pipeline is decided

It was the first step before [minishell](https://github.com/Skulay/minishell).

## Features

| Feature | Details |
|---|---|
| Two commands, one pipe | `cmd1` reads `infile`, `cmd2` writes `outfile`, both run at the same time |
| `PATH` lookup | Commands are searched in every directory of `$PATH`; a command containing a `/` is run as is |
| Quoted arguments | `"awk '{print \$1}'"` is split on spaces **outside** quotes, and the quotes are removed |
| Output file | Created if missing, truncated otherwise, with mode `0644` |
| Exit status | The status of `cmd2`, like a shell (`128 + n` if it was killed by signal `n`) |

### Error handling

| Case | Behaviour |
|---|---|
| Wrong number of arguments | Exits with status `1` |
| `infile` missing or unreadable | Prints the error, `cmd1` reads from `/dev/null`, `cmd2` still runs |
| `outfile` cannot be opened | Prints the error, `cmd2` exits with status `1` |
| Command not found | `<cmd>: command not found`, exit status `127` |
| Command found but `execve` fails | Prints the error, exit status `126` |
| Unclosed quote in a command | Exits with status `1` |

## Build

### Requirements

- Linux or macOS
- `cc` / `gcc` / `clang`
- `make`

### Compile

```bash
git clone <repository-url> pipex
cd pipex
make
```

My [libft](https://github.com/Skulay/libft) is included in the `libft/` folder and built automatically.

### Make rules

| Rule | Action |
|---|---|
| `make` / `make all` | Build `libft.a`, then `pipex` |
| `make clean` | Remove object files (pipex and libft) |
| `make fclean` | Remove object files, `libft.a` and the binary |
| `make re` | `fclean` then `all` |

## Usage

```
./pipex <infile> "<cmd1>" "<cmd2>" <outfile>
```

Each command and its arguments must be given as a single argument, between quotes.

## How it works

```
             ┌──────────── fork ────────────┐
             │                              │
             ▼                              ▼
     ┌──────────────┐    pipe fd[]   ┌──────────────┐
     │    child 1   │ ─────────────► │    child 2   │
     │ stdin=infile │  fd[1]  fd[0]  │ stdin=fd[0]  │
     │ stdout=fd[1] │                │ stdout=out   │
     │ execve cmd1  │                │ execve cmd2  │
     └──────────────┘                └──────────────┘
             │                              │
             └────────── waitpid ───────────┘
                           │
                           ▼
                exit status of cmd2
```

### 1. Init — `src/pipex_utils.c`

`init_data` opens `infile` (or `/dev/null` if that fails), splits both commands with `ft_split_quote`, and builds the list of `PATH` directories. Everything is stored in one struct, so it can be freed from any error path:

```c
typedef struct s_data
{
    int     fd[2];      // the pipe
    int     in;         // infile
    int     out;        // outfile
    int     status;     // status of the second child
    char    *outfile;
    char    **cmd_one;  // argv of cmd1
    char    **cmd_two;  // argv of cmd2
    char    **path;     // PATH directories, each ending with '/'
}   t_data;
```

### 2. Splitting with quotes — `src/ft_split_quote.c`

A version of `ft_split` that remembers whether it is inside `'…'` or `"…"`: spaces inside quotes do not split, and the outer quotes are removed from the result.

### 3. Finding the command — `src/path_utils.c`

`make_path` reads the `PATH=` line of the environment and splits it on `:`. `find_path_valid` joins each directory with the command name and returns the first one that `access(X_OK)` accepts.

### 4. Pipe, fork, exec — `ft_pipex`

1. `pipe` creates the two ends `fd[0]` (read) and `fd[1]` (write)
2. the first child plugs `infile` into its stdin and `fd[1]` into its stdout with `dup2`, then runs `cmd1`
3. the parent opens `outfile`, then forks the second child, which plugs `fd[0]` into its stdin and `outfile` into its stdout, then runs `cmd2`
4. the parent closes both ends of the pipe, otherwise `cmd2` would never see the end of its input, and waits for both children

### 5. Clean up — `src/free_utils.c`

`free_data` closes every fd that is still open and frees the command arrays and the `PATH` list. Children call it before exiting on an error.

## Project structure

```
.
├── includes/
│   └── pipex.h              # t_data and prototypes
├── libft/                   # my C library
├── makefile
└── src/
    ├── pipex.c              # main
    ├── pipex_utils.c        # init, children, pipe / fork / wait
    ├── path_utils.c         # PATH parsing and command lookup
    ├── ft_split_quote.c     # split that respects quotes
    ├── ft_split_quote_utils.c
    └── free_utils.c         # close fds, free memory, exit on error
```

## Examples

```console
$ printf 'hello world\nfoo bar\nhello 42\n' > infile

$ ./pipex infile "grep hello" "wc -l" outfile
$ cat outfile
2

$ ./pipex infile "cat" "awk '{print \$1}'" outfile
$ cat outfile
hello
foo
hello

$ ./pipex nofile "cat" "wc -l" outfile
nofile: No such file or directory
$ cat outfile
0

$ ./pipex infile "cat" "nosuchcmd" outfile
nosuchcmd: command not found
$ echo $?
127
```

## Testing

- every case compared with the same line run in `bash`: output, error messages and `$?`
- missing `infile`, read-only `outfile`, unknown commands, absolute paths (`/bin/ls`), empty commands
- `env -i ./pipex …` to run without `PATH`
- `valgrind --leak-check=full --track-fds=yes` to check for leaks and unclosed file descriptors, in the parent and in the children

## Resources

- `man 2` pages: `pipe`, `fork`, `execve`, `dup2`, `waitpid`, `access`, `open`
- [Bash Reference Manual: Pipelines](https://www.gnu.org/software/bash/manual/bash.html#Pipelines)

## Author

| Login | Work |
|---|---|
| **alehamad** | Everything |
