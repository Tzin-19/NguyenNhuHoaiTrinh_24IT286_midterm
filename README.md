# Midterm Project: Implement ls(1)

- **Course:** Systems Programming / UNIX Operating System
- **Student Name:** Nguyen Nhu Hoai Trinh
- **Student ID:** 24IT286
- **Tested Environment:** NetBSD 10.1 (GENERIC) & POSIX C99
- **GitHub Repository URL:** https://github.com/Tzin-19/NguyenNhuHoaiTrinh_24IT286_midterm

---

## 1. Project Overview

This project is an implementation of a simplified version of the UNIX `ls(1)` command from scratch in C, adhering to the specifications provided in the NetBSD 10.1 General Commands Manual. The implementation interacts directly with the filesystem using UNIX system calls (`stat`, `lstat`, `opendir`, `readdir`) to fetch file metadata, handle permissions, sort entries, and display directory contents cleanly.

---

## 2. Modular Architecture

The codebase is structured into clear, decoupled modules:

- **`include/flags.h` & `src/flags.c`**:
  Parses command-line options using `getopt(3)` and implements flag override precedence rules.
- **`include/file_info.h` & `src/file_info.c`**:
  Retrieves file attributes via `lstat(2)`, resolves UID/GID to User and Group names (`getpwuid`, `getgrgid`), and reads directory contents (`opendir`, `readdir`, `closedir`).
- **`include/sort.h` & `src/sort.c`**:
  Implements multi-criteria sorting algorithms (lexicographical, size `-S`, modification time `-t`, reverse `-r`, and unsorted `-f`).
- **`include/display.h` & `src/display.c`**:
  Formats permissions strings (`rwxr-xr-x`), long format listings (`-l`, `-n`), human-readable sizes (`-h`), block units (`-s`), and indicator suffixes (`-F`).
- **`src/main.c`**:
  Main controller orchestrating argument parsing, non-directory operand prioritization, directory listings, and recursive traversal (`-R`).
- **`Makefile`**:
  Build automation configured specifically for NetBSD BSD make.
- **`.gitignore`**:
  Configured to exclude binaries (`my_ls`) and object files (`*.o`).

---

## 3. Supported Flags & Features

All 19 flags required in the NetBSD manual are fully implemented:

| Flag | Description                                                                |    Status    |
| :--: | :------------------------------------------------------------------------- | :----------: |
| `-A` | List all entries except `.` and `..`                                       | ✅ Completed |
| `-a` | Include hidden files starting with `.`                                     | ✅ Completed |
| `-c` | Use `st_ctime` (status change time) for sorting/printing                   | ✅ Completed |
| `-d` | Treat directories as plain files (no recursion)                            | ✅ Completed |
| `-F` | Display indicators (`/` dir, `*` exec, `@` symlink, `=` socket, `\|` FIFO) | ✅ Completed |
| `-f` | Output is not sorted (automatically turns on `-a`)                         | ✅ Completed |
| `-h` | Display sizes in human-readable format (K, M, G). Overrides `-k`           | ✅ Completed |
| `-i` | Print file serial number (inode number)                                    | ✅ Completed |
| `-k` | Report block counts in kilobytes (1024-byte units)                         | ✅ Completed |
| `-l` | Long format listing                                                        | ✅ Completed |
| `-n` | Long format with numeric UID and GID                                       | ✅ Completed |
| `-q` | Print non-printable characters as `?`                                      | ✅ Completed |
| `-R` | Recursively list subdirectories                                            | ✅ Completed |
| `-r` | Reverse the order of the sort                                              | ✅ Completed |
| `-S` | Sort by file size, largest first                                           | ✅ Completed |
| `-s` | Display number of file system blocks used                                  | ✅ Completed |
| `-t` | Sort by modification time (most recent first)                              | ✅ Completed |
| `-u` | Use `st_atime` (last access time) for sorting/printing                     | ✅ Completed |
| `-w` | Force raw printing of non-printable characters                             | ✅ Completed |

### Override Precedence Handled:

- `-w` and `-q` override each other.
- `-l` and `-n` override each other.
- `-c` and `-u` override each other.
- `-R` and `-d` override each other.
- `-h` overrides `-k`.

---
