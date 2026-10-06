#ifndef FILE_INFO_H
#define FILE_INFO_H

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include "flags.h"

typedef struct
{
    char *name;         // Tên file (ví dụ: main.c)
    char *full_path;    // Đường dẫn đầy đủ (ví dụ: src/main.c)
    struct stat st;     // Thông tin stat/lstat
    char *owner_name;   // Tên chủ sở hữu (User)
    char *group_name;   // Tên nhóm (Group)
    char *link_target;  // Đường dẫn đích nếu là Symbolic link
    time_t active_time; // Thời gian dùng để sắp xếp/hiển thị (mtime, ctime hoặc atime)
} FileInfo;

FileInfo *create_file_info(const char *parent_dir, const char *name, const Options *opts);
void free_file_info(FileInfo *info);

// Đọc danh sách file trong 1 thư mục
int read_directory(const char *dir_path, const Options *opts, FileInfo ***file_list, int *count);

#endif // FILE_INFO_H