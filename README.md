# MyShell Program

## Introduction

MyShell is a simple command-line shell program written in C++. It provides basic functionalities for navigating the file system, managing files and directories, and executing commands. The shell supports commands such as changing directories (`cd`), listing directory contents (`ls`), removing files and directories (`rm`), moving files (`mv`), copying files and directories (`cp`), and displaying the current working directory (`pwd`).

## Features

### Navigation Commands

- `cd`: Change the current working directory.
  - Usage: `cd <directory>`
  - Additional Features:
    - `cd ..`: Move to the parent directory.
    - `cd -`: Move to the previous directory.
    - `cd ~`: Move to the home directory.
    - `cd help`: Display help for the `cd` command.

### Listing Commands

- `ls`: List the contents of a directory.
  - Usage: `ls [options] [directory]`
  - Options:
    - `-S`: Sort by file size.
    - `-h`: Display file sizes in a human-readable format.
    - `-t`: Sort by modification time.
    - `-l`: Display detailed information.
    - `-R`: Recursively list subdirectories.
  - Additional Features:
    - `ls help`: Display help for the `ls` command.

### Removal Commands

- `rm`: Remove files or directories.
  - Usage: `rm [options] <file/directory>`
  - Options:
    - `-i`: Interactive removal.
    - `-r`: Recursively remove directories.
  - Additional Features:
    - `rm *.txt`: Remove files with a specific extension.
    - `rm help`: Display help for the `rm` command.

### Move Command

- `mv`: Move or rename files and directories.
  - Usage: `mv <source> <destination>`
  - Additional Features:
    - `mv help`: Display help for the `mv` command.

### Copy Command

- `cp`: Copy files or directories.
  - Usage: `cp [options] <source> <destination>`
  - Options:
    - `-r`: Recursively copy directories.
  - Additional Features:
    - `cp help`: Display help for the `cp` command.

### Print Working Directory

- `pwd`: Display the current working directory.

### Exit Command

- `exit`: Exit the shell.

## Getting Started

To run the MyShell program, compile the source code and execute the generated executable.

```bash
g++ -o myshell myshell.cpp
./myshell
```

## Usage

Once the shell is running, enter commands at the prompt. Use the built-in commands or refer to the provided help messages for additional information.

```bash
$ cd /path/to/directory
$ ls -l
$ rm -r directory_name
$ mv file.txt /new/location
$ cp -r source_directory destination_directory
$ pwd
$ exit
```

## Notes

- The program uses the C++ `<filesystem>` library for file and directory operations.
- Some commands support additional options and features, which can be displayed using the `help` option.

Feel free to explore and customize the MyShell program based on your requirements.
