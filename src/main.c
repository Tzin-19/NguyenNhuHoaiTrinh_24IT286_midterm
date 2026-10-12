#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "flags.h"
#include "file_info.h"
#include "sort.h"
#include "display.h"

void process_directory(const char *dir_path, const Options *opts, int print_header)
{
    FileInfo **file_list = NULL;
    int count = 0;

    if (read_directory(dir_path, opts, &file_list, &count) != 0)
    {
        return;
    }

    sort_files(file_list, count, opts);

    if (print_header)
    {
        printf("%s:\n", dir_path);
    }

    display_files(file_list, count, opts, NULL);

    // Xử lý cờ đệ quy -R
    if (opts->flag_R)
    {
        for (int i = 0; i < count; i++)
        {
            FileInfo *f = file_list[i];
            if (S_ISDIR(f->st.st_mode))
            {
                if (strcmp(f->name, ".") != 0 && strcmp(f->name, "..") != 0)
                {
                    printf("\n");
                    process_directory(f->full_path, opts, 1);
                }
            }
        }
    }

    // Giải phóng bộ nhớ
    for (int i = 0; i < count; i++)
    {
        free_file_info(file_list[i]);
    }
    free(file_list);
}

int main(int argc, char *argv[])
{
    Options opts;
    int opt_ind = 1;

    if (parse_flags(argc, argv, &opts, &opt_ind) != 0)
    {
        return 1;
    }

    int num_args = argc - opt_ind;

    // Không truyền tham số -> Liệt kê thư mục hiện tại "."
    if (num_args == 0)
    {
        process_directory(".", &opts, 0);
    }
    else if (num_args == 1)
    {
        struct stat st;
        if (lstat(argv[opt_ind], &st) == 0 && S_ISDIR(st.st_mode) && !opts.flag_d)
        {
            process_directory(argv[opt_ind], &opts, 0);
        }
        else
        {
            FileInfo *info = create_file_info("", argv[opt_ind], &opts);
            FileInfo *list[1] = {info};
            display_files(list, 1, &opts, NULL);
            free_file_info(info);
        }
    }
    else
    {
        // Nhiều tham số: File thường in trước, thư mục in sau
        FileInfo **file_operands = (FileInfo **)malloc(num_args * sizeof(FileInfo *));
        char **dir_operands = (char **)malloc(num_args * sizeof(char *));
        int file_count = 0, dir_count = 0;

        for (int i = opt_ind; i < argc; i++)
        {
            struct stat st;
            if (lstat(argv[i], &st) == 0 && S_ISDIR(st.st_mode) && !opts.flag_d)
            {
                dir_operands[dir_count++] = argv[i];
            }
            else
            {
                file_operands[file_count++] = create_file_info("", argv[i], &opts);
            }
        }

        if (file_count > 0)
        {
            sort_files(file_operands, file_count, &opts);
            display_files(file_operands, file_count, &opts, NULL);
            for (int i = 0; i < file_count; i++)
                free_file_info(file_operands[i]);
            if (dir_count > 0)
                printf("\n");
        }
        free(file_operands);

        for (int i = 0; i < dir_count; i++)
        {
            process_directory(dir_operands[i], &opts, 1);
            if (i < dir_count - 1)
                printf("\n");
        }
        free(dir_operands);
    }

    return 0;
}