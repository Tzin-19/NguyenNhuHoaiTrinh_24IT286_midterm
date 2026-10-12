#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sort.h"

static const Options *current_opts = NULL;

static int compare_files(const void *a, const void *b)
{
    const FileInfo *fileA = *(const FileInfo **)a;
    const FileInfo *fileB = *(const FileInfo **)b;
    int res = 0;

    // 1. Sắp xếp theo Kích thước (-S)
    if (current_opts->flag_S)
    {
        if (fileA->st.st_size > fileB->st.st_size)
            res = -1;
        else if (fileA->st.st_size < fileB->st.st_size)
            res = 1;
        else
            res = strcmp(fileA->name, fileB->name);
    }
    // 2. Sắp xếp theo Thời gian (-t)
    else if (current_opts->flag_t)
    {
        if (fileA->active_time > fileB->active_time)
            res = -1;
        else if (fileA->active_time < fileB->active_time)
            res = 1;
        else
            res = strcmp(fileA->name, fileB->name);
    }
    // 3. Mặc định: Sắp xếp theo tên (Lexicographical)
    else
    {
        res = strcmp(fileA->name, fileB->name);
    }

    // Đảo ngược kết quả nếu có cờ -r
    if (current_opts->flag_r)
    {
        res = -res;
    }

    return res;
}

void sort_files(FileInfo **files, int count, const Options *opts)
{
    // Nếu có cờ -f -> Không sắp xếp
    if (opts->flag_f)
        return;

    current_opts = opts;
    qsort(files, count, sizeof(FileInfo *), compare_files);
}