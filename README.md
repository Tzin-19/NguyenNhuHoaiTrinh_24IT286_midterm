# Midterm Project: Implement ls(1)

- **Student Name:** Nguyen Nhu Hoai Trinh
- **Student ID:** 24IT286
- **Tested Environment:** NetBSD 10.1 (GENERIC)
- **GitHub URL:** https://github.com/Tzin-19/NguyenNhuHoaiTrinh_24IT286_midterm

## 1. Project Overview

Implementation of the UNIX `ls(1)` command in C, following the NetBSD manual page.

## 2. Modular Structure

- `include/flags.h`, `src/flags.c`: Parse command-line options and handle precedence.
- `include/file_info.h`, `src/file_info.c`: Read directory entries, stat/lstat metadata, user/group IDs.
- `include/sort.h`, `src/sort.c`: Sort files (by name, size `-S`, time `-t`, reverse `-r`).
- `include/display.h`, `src/display.c`: Format permissions, long format (`-l`, `-n`), human-readable sizes (`-h`), file indicators (`-F`).
- `src/main.c`: Program flow and recursive traversal (`-R`).

## 3. Supported Flags

Implemented all 19 flags: `-A`, `-a`, `-c`, `-d`, `-F`, `-f`, `-h`, `-i`, `-k`, `-l`, `-n`, `-q`, `-R`, `-r`, `-S`, `-s`, `-t`, `-u`, `-w`.

## 4. How to Compile & Run

```bash
make
./my_ls -la
./my_ls -lhF
./my_ls -R
```
