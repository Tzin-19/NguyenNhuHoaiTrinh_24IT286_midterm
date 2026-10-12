#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include "file_info.h"

#if defined(__unix__) || defined(__APPLE__) || defined(__linux__)
#include <pwd.h>
#include <grp.h>
#endif

// Tạo và nạp thuộc tính cho 1 file
FileInfo *create_file_info(const char *parent_dir, const char *name, const Options *opts)
{
    FileInfo *info = (FileInfo *)malloc(sizeof(FileInfo));
    if (!info)
        return NULL;

    info->name = strdup(name);

    // Ghép đường dẫn đầy đủ full_path
    if (parent_dir == NULL || strlen(parent_dir) == 0)
    {
        info->full_path = strdup(name);
    }
    else
    {
        size_t path_len = strlen(parent_dir) + strlen(name) + 2;
        info->full_path = (char *)malloc(path_len);
        if (strcmp(parent_dir, "/") == 0)
        {
            snprintf(info->full_path, path_len, "/%s", name);
        }
        else
        {
            snprintf(info->full_path, path_len, "%s/%s", parent_dir, name);
        }
    }

    // Lấy thuộc tính file bằng lstat (không đi theo link)
    if (lstat(info->full_path, &(info->st)) != 0)
    {
        memset(&(info->st), 0, sizeof(struct stat));
    }

    // Chọn mốc thời gian mtime/ctime/atime dựa theo cờ -c hoặc -u
    if (opts->flag_c)
    {
        info->active_time = info->st.st_ctime;
    }
    else if (opts->flag_u)
    {
        info->active_time = info->st.st_atime;
    }
    else
    {
        info->active_time = info->st.st_mtime;
    }

    // Lấy tên User và Group
#if defined(__unix__) || defined(__APPLE__) || defined(__linux__)
    struct passwd *pw = getpwuid(info->st.st_uid);
    info->owner_name = pw ? strdup(pw->pw_name) : NULL;

    struct group *gr = getgrgid(info->st.st_gid);
    info->group_name = gr ? strdup(gr->gr_name) : NULL;
#else
    char uid_buf[16], gid_buf[16];
    snprintf(uid_buf, sizeof(uid_buf), "%u", (unsigned int)info->st.st_uid);
    snprintf(gid_buf, sizeof(gid_buf), "%u", (unsigned int)info->st.st_gid);
    info->owner_name = strdup(uid_buf);
    info->group_name = strdup(gid_buf);
#endif

    // Lấy đường dẫn đích nếu là Symbolic Link
    info->link_target = NULL;
    if (S_ISLNK(info->st.st_mode))
    {
        char buf[1024];
        ssize_t len = readlink(info->full_path, buf, sizeof(buf) - 1);
        if (len != -1)
        {
            buf[len] = '\0';
            info->link_target = strdup(buf);
        }
    }

    return info;
}

// Giải phóng bộ nhớ động
void free_file_info(FileInfo *info)
{
    if (!info)
        return;
    if (info->name)
        free(info->name);
    if (info->full_path)
        free(info->full_path);
    if (info->owner_name)
        free(info->owner_name);
    if (info->group_name)
        free(info->group_name);
    if (info->link_target)
        free(info->link_target);
    free(info);
}

// Đọc tất cả các file có trong 1 thư mục
int read_directory(const char *dir_path, const Options *opts, FileInfo ***file_list, int *count)
{
    DIR *dir = opendir(dir_path);
    if (!dir)
    {
        fprintf(stderr, "ls: %s: %s\n", dir_path, strerror(errno));
        return -1;
    }

    struct dirent *entry;
    int capacity = 16;
    *count = 0;
    *file_list = (FileInfo **)malloc(capacity * sizeof(FileInfo *));

    while ((entry = readdir(dir)) != NULL)
    {
        // Xử lý cờ ẩn -a và -A
        if (entry->d_name[0] == '.')
        {
            if (!opts->flag_a && !opts->flag_A && !opts->flag_f)
            {
                continue;
            }
            if (opts->flag_A && !opts->flag_a)
            {
                if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                {
                    continue;
                }
            }
        }

        FileInfo *info = create_file_info(dir_path, entry->d_name, opts);
        if (info)
        {
            if (*count >= capacity)
            {
                capacity *= 2;
                *file_list = (FileInfo **)realloc(*file_list, capacity * sizeof(FileInfo *));
            }
            (*file_list)[(*count)++] = info;
        }
    }

    closedir(dir);
    return 0;
}